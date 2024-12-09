#include <Game/GameLogicSystem.h>

#include <Game/GameObjects.h>
#include <Game/PlayField.h>
#include <Game/TetrominoComponent.h>
#include <Game/BlockComponent.h>
#include <Game/PlacementComponent.h>
#include <Game/GravityComponent.h>
#include <Game/GameState.h>
#include <Game/Control/TetrominoMovementComponent.h>
#include <Game/Control/PlayfieldActions.h>

#include <DFW/GameWorld/TransformComponent.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>
#include <DFW/Modules/ECS/Internal/EntityRelationComponent.h>

#include <DFW/CoreSystems/CoreServices.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>

#include <DFW/GameWorld/Graphics/DebugRenderSystem.h>

#include <ranges>
#include <algorithm>

namespace Tetriys
{
    bool IsTetrominoBlockedAtCoordinate(TetrominoComponent const& a_tetromino, struct PlayField const& a_playfield, BlockCoordinate const& a_coordinate)
    {
        // Check at new coordinates if there are blocking blocks.
        bool can_tetromino_be_moved(true);
        for (int32 index(0); index < a_tetromino.blocks.size(); index++)
        {
            BlockCoordinate const coordinate_to_check = a_coordinate + a_tetromino.block_components[index]->local_offset_coordinate;

            if (!a_playfield.IsValidGridCoordinate(coordinate_to_check))
            {
                can_tetromino_be_moved = false;
                break;
            }

            // Check for blocks other than the blocks of the tetromino.
            PlayFieldDataEntry const& data_entry = a_playfield.GetDataEntry(coordinate_to_check);
            if (data_entry.block.IsEntityValid() && !data_entry.block.IsChildOfEntity(a_tetromino.GetOwner()))
            {
                can_tetromino_be_moved = false;
                break;
            }
        }

        // If can move, thus not blocked.
        return !can_tetromino_be_moved;
    }

    bool IsTetrominoBlockedAtCoordinate(DFW::Entity const& a_tetromino, BlockCoordinate const& a_coordinate)
    {
        return IsTetrominoBlockedAtCoordinate(a_tetromino.GetComponent<TetrominoComponent>(), a_tetromino.GetComponent<PlayFieldRef>().Get(), a_coordinate);
    }

    std::array<BlockCoordinate, 4> SimulateTetrominoPlacementLocation(DFW::Entity const& a_tetromino)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        PlayField const& playfield = a_tetromino.GetComponent<PlayFieldRef>().Get();

        bool simulate(true);
        BlockCoordinate const simulation_start_coordinate = tetromino.block_components[tetromino.origin_block_index]->grid_coordinate;
        BlockCoordinate current_unblocked_coordinate = simulation_start_coordinate;

        // Continue to simulate until tetromino is blocked by other blocks or the bounds of the playfield.
        while (simulate)
        {
            BlockCoordinate const coordinate_to_check = current_unblocked_coordinate + TETRIYS_GRAVITY_TICK;
            if (IsTetrominoBlockedAtCoordinate(a_tetromino, coordinate_to_check))
            {
                simulate = false;
            }
            else
            {
                current_unblocked_coordinate = coordinate_to_check;
            }
        }

        std::array<BlockCoordinate, 4> placement_locations;
        placement_locations[0] = current_unblocked_coordinate + tetromino.block_components[0]->local_offset_coordinate;
        placement_locations[1] = current_unblocked_coordinate + tetromino.block_components[1]->local_offset_coordinate;
        placement_locations[2] = current_unblocked_coordinate + tetromino.block_components[2]->local_offset_coordinate;
        placement_locations[3] = current_unblocked_coordinate + tetromino.block_components[3]->local_offset_coordinate;

        return placement_locations;
    }

    void PlayDirector::Init(DFW::DECS::EntityRegistry& a_registry)
    {
        ECSEventHandler().RegisterCallback<TetrominoSpawnedEvent, &PlayDirector::OnTetrominoSpawnedEvent>(this);
        ECSEventHandler().RegisterCallback<TetrominoPlacedEvent, &PlayDirector::OnTetrominoPlacedEvent>(this);
        ECSEventHandler().RegisterCallback<PlayfieldLineClearedEvent, &PlayDirector::OnPlayfieldLineClearedEvent>(this);
    }

    void PlayDirector::Terminate(DFW::DECS::EntityRegistry& a_registry)
    {
        ECSEventHandler().UnregisterCallback<TetrominoSpawnedEvent, &PlayDirector::OnTetrominoSpawnedEvent>(this);
        ECSEventHandler().UnregisterCallback<TetrominoPlacedEvent, &PlayDirector::OnTetrominoPlacedEvent>(this);
        ECSEventHandler().UnregisterCallback<PlayfieldLineClearedEvent, &PlayDirector::OnPlayfieldLineClearedEvent>(this);
    }

    void PlayDirector::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, game_state_comp, playfield_ref]
            : a_registry.ENTT().view<GameStateComponent, PlayField>().each())
        {
            if (game_state_comp.game_state == GameState::START_GAME)
            {
                game_state_comp.game_state = GameState::PLAY_GAME;
                game_state_comp.play_state = PlayState::STARTING;
            }

            switch (game_state_comp.play_state)
            {
                case PlayState::STARTING:
                {
                    game_state_comp.play_state = PlayState::SPAWNING;
                    break;
                }
            }
        }
    }

    void PlayDirector::OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event)
    {
        PlayField& playfield = a_event.spawned_tetromino.GetComponent<PlayFieldRef>().Get();
        DFW::Entity game_entry = playfield.GetOwner();
        game_entry.GetComponent<GameStateComponent>().play_state = PlayState::PLACING;
    }

    void PlayDirector::OnTetrominoPlacedEvent(TetrominoPlacedEvent& a_event)
    {
        PlayField& playfield = a_event.placed_tetromino.GetComponent<PlayFieldRef>().Get();
        DFW::Entity game_entry = playfield.GetOwner();
        game_entry.GetComponent<GameStateComponent>().play_state = PlayState::CHECKING_LINE_CLEARS;
    }

    void PlayDirector::OnPlayfieldLineClearedEvent(PlayfieldLineClearedEvent& a_event)
    {
        a_event.game_entry.GetComponent<GameStateComponent>().play_state = PlayState::SPAWNING;
    }

    void GravitySystem::Init(DFW::DECS::EntityRegistry& a_registry)
    {
        _game_clock = DFW::CoreService::GetGameClock();

        ECSEventHandler().RegisterCallback<TetrominoSpawnedEvent, &GravitySystem::OnTetrominoSpawnedEvent>(this);
        ECSEventHandler().RegisterCallback<TetrominoPlacedEvent, &GravitySystem::OnTetrominoPlacedEvent>(this);
    }

    void GravitySystem::Terminate(DFW::DECS::EntityRegistry& a_registry)
    {
        ECSEventHandler().UnregisterCallback<TetrominoSpawnedEvent, &GravitySystem::OnTetrominoSpawnedEvent>(this);
        ECSEventHandler().UnregisterCallback<TetrominoPlacedEvent, &GravitySystem::OnTetrominoPlacedEvent>(this);
    }

    void GravitySystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, gravity_component, tetromino_movement_component] : a_registry.ENTT().view<GravityComponent, TetrominoMovementComponent>().each())
        {
            if (!tetromino_movement_component.is_dropping)
            {
                bool const apply_gravity_this_frame = gravity_component.current_between_fall_time >= gravity_component.max_time_between_fall_tick;
                if (apply_gravity_this_frame)
                {
                    tetromino_movement_component.desired_movement_action.coordinate = TETRIYS_GRAVITY_TICK;
                    gravity_component.current_between_fall_time = 0;
                }

                gravity_component.current_between_fall_time += _game_clock->GetLastFrameDeltaTime();
            }
            else
            {
                gravity_component.current_between_fall_time = 0;
            }
        }
    }

    void GravitySystem::OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event)
    {
        a_event.spawned_tetromino.AddComponent<GravityComponent>();
    }

    void GravitySystem::OnTetrominoPlacedEvent(TetrominoPlacedEvent& a_event)
    {
        a_event.placed_tetromino.DeleteComponent<GravityComponent>();
    }

    void LockTetrominoPlacementSystem::Init(DFW::DECS::EntityRegistry& a_registry)
    {
        _game_clock = DFW::CoreService::GetGameClock();

        ECSEventHandler().RegisterCallback<TetrominoSpawnedEvent, &LockTetrominoPlacementSystem::OnTetrominoSpawnedEvent>(this);
    }

    void LockTetrominoPlacementSystem::Terminate(DFW::DECS::EntityRegistry& a_registry)
    {
        ECSEventHandler().UnregisterCallback<TetrominoSpawnedEvent, &LockTetrominoPlacementSystem::OnTetrominoSpawnedEvent>(this);
    }

    void LockTetrominoPlacementSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, placement_comp, tetromino_movement_comp, tetromino_comp, playfield_ref]
            : a_registry.ENTT().view<PlacementComponent, TetrominoMovementComponent, TetrominoComponent, PlayFieldRef>().each())
        {
            BlockCoordinate const& tetromino_coordinate = tetromino_comp.block_components[tetromino_comp.origin_block_index]->grid_coordinate;
            placement_comp.check_placement_time = IsTetrominoBlockedAtCoordinate(tetromino_comp, playfield_ref.Get(), tetromino_coordinate + TETRIYS_GRAVITY_TICK);

            if (placement_comp.check_placement_time && !tetromino_movement_comp.is_strafing)
            {
                bool const lock_tetromino_in_place = placement_comp.current_placement_time >= placement_comp.max_placement_time;
                if (lock_tetromino_in_place)
                {
                    DFW::Entity entity(e, a_registry);
                    entity.DeleteComponent<PlacementComponent>();

                    ECSEventHandler().Broadcast<TetrominoPlacedEvent>(entity);
                }

                placement_comp.current_placement_time += _game_clock->GetLastFrameDeltaTime();
            }
            else
            {
                placement_comp.current_placement_time = 0;
            }
        }
    }

    void LockTetrominoPlacementSystem::OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event)
    {
        a_event.spawned_tetromino.AddComponent<PlacementComponent>();
    }

    void SpawnSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, game_state_comp, game_id_comp, playfield]
            : a_registry.ENTT().view<GameStateComponent, GameNameIDComponent, PlayField>().each())
        {
            if (game_state_comp.play_state == PlayState::SPAWNING)
            {
                // TODO Implement proper tetromino spawnbag functionality.
                static int32 test(0);
                constexpr int32 num_tetromino_types(7);
                if (test >= num_tetromino_types)
                    test = 1;
                else
                    test++;

                DFW::Entity spawned_tetromino = GameObjects::CreateTetrominoEntity(a_registry, static_cast<TetrominoType>(test));
                spawned_tetromino.AddComponent<TetrminoInsertAction>(BlockCoordinate(5, 20));
                spawned_tetromino.AddComponent<PlayFieldRef>(playfield);

                DFW::Entity game_entry(e, a_registry);
                spawned_tetromino.SetParent(game_entry);

                ECSEventHandler().Broadcast<TetrominoSpawnedEvent>(spawned_tetromino, game_id_comp.game_id);
            }
        }
    }

    void LineClearSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, game_state_comp, game_id_comp, playfield]
            : a_registry.ENTT().view<GameStateComponent, GameNameIDComponent, PlayField>().each())
        {
            if (game_state_comp.play_state != PlayState::CHECKING_LINE_CLEARS)
            {
                break;
            }

            int32 const maximum_possible_line_clears(4);
            std::vector<int32> full_row_indicies;
            full_row_indicies.reserve(maximum_possible_line_clears);

            for (int32 row_index(0); row_index < TETRIYS_GRID_HEIGHT; row_index++)
            {
                bool is_row_full_with_blocks(true);

                std::array<PlayFieldDataEntry*, TETRIYS_GRID_WIDTH> row_ref = playfield.GetRowRef(row_index);
                for (PlayFieldDataEntry* data_entry : row_ref)
                {
                    if (!data_entry->block.IsEntityValid())
                    {
                        is_row_full_with_blocks = false;
                        break;
                    }
                }

                if (is_row_full_with_blocks)
                {
                    full_row_indicies.emplace_back(row_index);

                    for (PlayFieldDataEntry* data_entry : row_ref)
                    {
                        DFW::Entity& block_parent = data_entry->block.GetComponent<DFW::DECS::EntityRelationComponent>().parent;
                        block_parent.AddComponent<ClearedBlockInTetrominoTag>();

                        data_entry->DestroyBlockEntity();
                    }
                }
            }

            PlayfieldLineClearedEvent event;
            event.cleared_line_indices = full_row_indicies;
            event.game_entry = DFW::Entity(e, a_registry);
            ECSEventHandler().Broadcast<PlayfieldLineClearedEvent>(event);

            // Back out if there are no full rows.
            if (full_row_indicies.empty())
            {
                break;
            }

            // Push rows down after line clear - start from high to low to try and reduce row swaps.
            std::ranges::sort(full_row_indicies, std::ranges::greater());

            auto SwapRows = [&playfield](
                std::array<PlayFieldDataEntry*, TETRIYS_GRID_WIDTH> a_bottom_row,
                std::array<PlayFieldDataEntry*, TETRIYS_GRID_WIDTH> a_top_row)
                {
                    for (int32 item_index(0); item_index < TETRIYS_GRID_WIDTH; item_index++)
                    {
                        PlayFieldDataEntry* bottom_data_entry = a_bottom_row.at(item_index);
                        PlayFieldDataEntry* top_data_entry = a_top_row.at(item_index);
                        std::swap(bottom_data_entry->block, top_data_entry->block);

                        if (bottom_data_entry->block)
                            bottom_data_entry->SyncBlockEntity();
                        if (top_data_entry->block)
                            top_data_entry->SyncBlockEntity();
                    }
                };

            for (int32 row_index : full_row_indicies)
            {
                for (row_index; row_index + 1 < TETRIYS_GRID_HEIGHT; row_index++)
                {
                    SwapRows(playfield.GetRowRef(row_index), playfield.GetRowRef(row_index + 1));
                }
            }
        }
    }

    void LineClearSystem::PostUpdate(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, tetromino_component, cleared_tetromino_tag] : a_registry.ENTT().view<TetrominoComponent const, ClearedBlockInTetrominoTag const>(entt::exclude<TetrminoInsertAction>).each())
        {
            bool should_cleanup_entity(false);
            DFW::Entity tetromino(e, a_registry);

            //DFW_INFOLOG("----- {}", tetromino_component.GetOwner().GetName());
            for (DFW::Entity const& block : tetromino_component.blocks)
            {
                DFW::EntityHandle const handle = block ? block.GetHandle() : DFW::DECS::DFW_NULL_ENTITY_HANDLE;
                std::string const& name = block ? block.GetName() : "INVALID";
                //DFW_INFOLOG("{}-{}-{}", handle, name, block.IsEntityValid());
                should_cleanup_entity = !block.IsEntityValid();
            }

            if (!tetromino_component.blocks[0].IsEntityValid()
                && !tetromino_component.blocks[1].IsEntityValid()
                && !tetromino_component.blocks[2].IsEntityValid()
                && !tetromino_component.blocks[3].IsEntityValid())
            {
                //DFW_INFOLOG("Destroyed Tetromino: {}", e);
                tetromino.DestroySelf();
            }

            tetromino.DeleteComponent<ClearedBlockInTetrominoTag>();
        }
    }

    void TetrominoPlacementVisualizerSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, tetromino_movement_comp, playfield_ref]
            : a_registry.ENTT().view<TetrominoMovementComponent const, PlayFieldRef const>().each())
        {
            PlayField const& playfield = playfield_ref.Get();

            std::array<glm::vec3, 4> placement_world_coordinates;
            std::array<BlockCoordinate, 4> const placement_locations = SimulateTetrominoPlacementLocation(DFW::Entity(e, a_registry));
            placement_world_coordinates[0] = playfield.GetDataEntry(placement_locations[0]).block_world_position;
            placement_world_coordinates[1] = playfield.GetDataEntry(placement_locations[1]).block_world_position;
            placement_world_coordinates[2] = playfield.GetDataEntry(placement_locations[2]).block_world_position;
            placement_world_coordinates[3] = playfield.GetDataEntry(placement_locations[3]).block_world_position;

            DFW::DebugDrawSettings const draw_settings(DFW::ColourRGBA::LightGrey, true);
            DFW::DebugRenderSystem* debug_renderer = SystemManager().GetSystem<DFW::DebugRenderSystem>();
            debug_renderer->DrawCube(DFW::Transform(placement_world_coordinates[0]), TETRIYS_BLOCK_SPACING * 0.5f, draw_settings);
            debug_renderer->DrawCube(DFW::Transform(placement_world_coordinates[1]), TETRIYS_BLOCK_SPACING * 0.5f, draw_settings);
            debug_renderer->DrawCube(DFW::Transform(placement_world_coordinates[2]), TETRIYS_BLOCK_SPACING * 0.5f, draw_settings);
            debug_renderer->DrawCube(DFW::Transform(placement_world_coordinates[3]), TETRIYS_BLOCK_SPACING * 0.5f, draw_settings);
        }
    }

} // End of namespace ~ Tetriys.
