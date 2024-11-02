#include <Game/GameLogicSystem.h>

#include <Game/PlayField.h>
#include <Game/TetrominoComponent.h>
#include <Game/BlockComponent.h>
#include <Game/PlacementComponent.h>
#include <Game/GravityComponent.h>
#include <Game/GameEvents.h>
#include <Game/Control/TetrominoMovementComponent.h>
#include <Game/Control/PlayfieldActions.h>

#include <DFW/GameWorld/TransformComponent.h>
#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

#include <DFW/CoreSystems/CoreServices.h>
#include <DFW/CoreSystems/Events/EventDispatcher.h>

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

    BlockCoordinate SimulateTetrominoFallLocation(DFW::Entity const& a_tetromino)
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

        return current_unblocked_coordinate;
    }

    void GameLogicSystem::Init(DFW::DECS::EntityRegistry& a_registry)
    {
        _game_clock = DFW::CoreService::GetGameClock();
    }

    void GameLogicSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        ExecuteGravity(a_registry);
        ExecuteTetrominoPlacementChecks(a_registry);
    }

    void GameLogicSystem::ExecuteGravity(DFW::DECS::EntityRegistry& a_registry)
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

    void GameLogicSystem::ExecuteTetrominoPlacementChecks(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, placement_comp, tetromino_movement_comp, tetromino_comp, playfield_ref] 
            : a_registry.ENTT().view<PlacementComponent, TetrominoMovementComponent, TetrominoComponent, PlayFieldRef>().each())
        {
            BlockCoordinate const& tetromino_coordinate = tetromino_comp.block_components[tetromino_comp.origin_block_index]->grid_coordinate;
            placement_comp.check_placement_time = IsTetrominoBlockedAtCoordinate(tetromino_comp, playfield_ref.Get(), tetromino_coordinate + TETRIYS_GRAVITY_TICK);

            if (placement_comp.check_placement_time && !tetromino_movement_comp.has_moved)
            {
                bool const lock_tetromino_in_place = placement_comp.current_placement_time >= placement_comp.max_placement_time;
                if (lock_tetromino_in_place)
                {
                    DFW::Entity entity(e, a_registry);
                    entity.DeleteComponent<TetrominoMovementComponent>();
                    entity.DeleteComponent<GravityComponent>();
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

} // End of namespace ~ Tetriys.
