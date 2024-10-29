#include <Game/Control/TetrominoControlSystem.h>

#include <Game/BlockComponent.h>
#include <Game/TetrominoComponent.h>
#include <Game/PlayField.h>
#include <Game/Control/PlayFieldControlSystem.h>
#include <Game/Control/TetrominoMovementComponent.h>

#include <DFW/Modules/ECS/ECSModule.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

#include <functional>

namespace Tetriys
{
    namespace Detail
    {
        bool WantsToTeleport(TetrominoMovementComponent const& a_tetromino)
        {
            return a_tetromino.desired_movement_action.is_absolute_coordinate && a_tetromino.desired_movement_action.coordinate != glm::ivec2(0.0f);
        }

        bool WantsToStrafe(TetrominoMovementComponent const& a_tetromino)
        {
            return a_tetromino.desired_movement_action.coordinate != glm::ivec2(0.0f);
        }

        bool WantsToRotate(TetrominoMovementComponent const& a_tetromino)
        {
            return a_tetromino.desired_rotation_action != TetrominoRotation::None;
        }

        bool WantsToSoftDrop(TetrominoMovementComponent const& a_tetromino)
        {
            return a_tetromino.wants_to_soft_drop == true;
        }

        bool WantsToHardDrop(TetrominoMovementComponent const& a_tetromino)
        {
            return a_tetromino.wants_to_hard_drop == true;
        }
    }

    void TetrominoControlSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        // Destroy Tetromino
        for (auto&& [e, tetromino_destroy_tag, playfield_reference] : a_registry.ENTT().view<TetrominoDestroyTag, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            DestroyTetromino(playfield_reference.Get(), entity);
            entity.DeleteComponent<TetrominoDestroyTag>();
        }

        // Remove Tetromino
        for (auto&& [e, tetromino_remove_tag, playfield_reference] : a_registry.ENTT().view<TetrominoRemoveTag, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            RemoveTetromino(playfield_reference.Get(), entity);
            entity.DeleteComponent<TetrominoRemoveTag>();
        }

        // Insert Tetromino
        for (auto&& [e, tetromino_insertion, playfield_reference] : a_registry.ENTT().view<TetrminoInsertAction, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            InsertTetromino(playfield_reference.Get(), entity, tetromino_insertion.coordinate);
            entity.DeleteComponent<TetrminoInsertAction>();
        }   

        // Move Tetromino
        for (auto&& [e, tetromino_movement_comp, playfield_reference] : a_registry.ENTT().view<TetrominoMovementComponent, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            PlayField& playfield = playfield_reference.Get();

            tetromino_movement_comp.is_strafing = false;
            tetromino_movement_comp.is_rotating = false;
            tetromino_movement_comp.is_dropping = false;

            if (Detail::WantsToTeleport(tetromino_movement_comp))
            {
                MoveTetromino(playfield, entity, tetromino_movement_comp.desired_movement_action.coordinate);
                break;
            }

            if (Detail::WantsToStrafe(tetromino_movement_comp))
            {
                TranslateTetromino(playfield, entity, tetromino_movement_comp.desired_movement_action.coordinate);
                tetromino_movement_comp.is_strafing = true;
            }

            if (Detail::WantsToRotate(tetromino_movement_comp))
            {
                RotateTetromino(playfield, entity, tetromino_movement_comp.desired_rotation_action);
                tetromino_movement_comp.is_rotating= true;
            }

            if (Detail::WantsToSoftDrop(tetromino_movement_comp))
            {
                TranslateTetromino(playfield, entity, tetromino_movement_comp.desired_movement_action.coordinate);
                tetromino_movement_comp.is_dropping = true;
            }

            if (Detail::WantsToHardDrop(tetromino_movement_comp))
            {
                // TODO
                // tetromino_movement_comp.is_dropping = true;
            }

            tetromino_movement_comp.ResetActions();
        }
    }

    void TetrominoControlSystem::InsertTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        for (int32 index(0); index < tetromino.blocks.size(); index++)
            InsertBlockInGrid(a_playfield, tetromino.blocks[index], a_coordinate + tetromino.block_components[index]->local_offset_coordinate, false);

        if (!a_tetromino.HasComponents<PlayFieldRef>())
            a_tetromino.AddComponent<PlayFieldRef>(a_playfield);
    }

    void TetrominoControlSystem::DestroyTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        for (int32 index(0); index < tetromino.blocks.size(); index++)
            DestroyBlockInGrid(a_playfield, tetromino.block_components[index]->grid_coordinate);
    }

    void TetrominoControlSystem::RemoveTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        for (int32 index(0); index < tetromino.blocks.size(); index++)
            RemoveBlockInGrid(a_playfield, tetromino.block_components[index]->grid_coordinate);
    }

    void TetrominoControlSystem::MoveTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();

        // Calculate coordinates after move.
        std::array<BlockCoordinate, 4> new_grid_coordinates;
        BlockCoordinate const& new_tetromino_origin = a_coordinate;

        for (int32 index(0); index < tetromino.blocks.size(); index++)
        {
            new_grid_coordinates[index] = new_tetromino_origin + tetromino.block_components[index]->local_offset_coordinate;
        }

        // Check if at new coordinates there are blocking blocks.
        bool can_tetromino_be_moved(true);
        for (BlockCoordinate const& coordinate : new_grid_coordinates)
        {
            if (!a_playfield.IsValidGridCoordinate(coordinate))
            {
                can_tetromino_be_moved = false;
                break;
            }

            // Check for blocks other than the blocks of the tetromino.
            PlayFieldDataEntry const& data_entry = a_playfield.GetDataEntry(coordinate);
            if (data_entry.block.IsEntityValid() && !data_entry.block.IsChildOfEntity(a_tetromino))
            {
                can_tetromino_be_moved = false;
            }
        }

        if (can_tetromino_be_moved)
        {
            // Remove tetromino block entries from the grid, preventing insertion blockage.
            for (BlockComponent const* block_component : tetromino.block_components)
            {
                RemoveBlockInGrid(a_playfield, block_component->grid_coordinate);
            }

            // Insert the blocks back into the grid at the new coordinates.
            for (int32 index(0); index < tetromino.blocks.size(); index++)
            {
                InsertBlockInGrid(a_playfield, tetromino.blocks[index], new_grid_coordinates[index], false);
            }
        }
    }

    void TetrominoControlSystem::TranslateTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate_offset)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        MoveTetromino(a_playfield, a_tetromino, tetromino.block_components[tetromino.origin_block_index]->grid_coordinate + a_coordinate_offset);
    }

    void TetrominoControlSystem::RotateTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, TetrominoRotation const& a_rotation)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();

        std::function<BlockCoordinate(BlockCoordinate const&)> RotateFunc;
        glm::mat2x2 rotation;

        switch (a_rotation)
        {
        case (TetrominoRotation::Clockwise):
        {
            rotation = { 0, -1, 1, 0 };
            RotateFunc = [&rotation](BlockCoordinate const& a_coordinate) -> BlockCoordinate { return rotation * a_coordinate; };
            break;
        }
        case (TetrominoRotation::CounterClockwise):
        {
            rotation = { 0, 1, -1, 0 };
            RotateFunc = [&rotation](BlockCoordinate const& a_coordinate) -> BlockCoordinate { return rotation * a_coordinate; };
            break;
        }
        case (TetrominoRotation::Clockwise180):
        {
            rotation = { 0, 1, -1, 0 };
            RotateFunc = [&rotation](BlockCoordinate const& a_coordinate) -> BlockCoordinate { return rotation * (rotation * a_coordinate); };
            break;
        }
        }

        // Calculate coordinates after the rotation.
            // TODO Apply Offset Coordinates (see SRS)
        std::array<BlockCoordinate, 4> new_grid_coordinates;
        std::array<BlockCoordinate, 4> new_local_offset_coordinates;
        BlockCoordinate const& tetromino_origin = tetromino.block_components[tetromino.origin_block_index]->grid_coordinate;

        for (int32 index(0); index < tetromino.blocks.size(); index++)
        {
            BlockCoordinate const rotated_local_offset_coordinate = RotateFunc(tetromino.block_components[index]->local_offset_coordinate);
            new_local_offset_coordinates[index] = rotated_local_offset_coordinate;
            new_grid_coordinates[index] = tetromino_origin + rotated_local_offset_coordinate;
        }

        // Check if at new coordinates there are blocking blocks.
        bool can_tetromino_be_rotated(true);
        for (BlockCoordinate const& coordinate : new_grid_coordinates)
        {
            if (!a_playfield.IsValidGridCoordinate(coordinate))
            {
                can_tetromino_be_rotated = false;
                break;
            }

            // Check for blocks other than the blocks of the tetromino.
            PlayFieldDataEntry const& data_entry = a_playfield.GetDataEntry(coordinate);
            if (data_entry.block.IsEntityValid() && !data_entry.block.IsChildOfEntity(a_tetromino))
            {
                if (!data_entry.block.IsChildOfEntity(a_tetromino))
                    can_tetromino_be_rotated = false;
            }
        }

        // If rotation is not possible, attempt to find other possible rotations using offset table (see SRS)
            // TODO

        if (can_tetromino_be_rotated)
        {
            // Remove tetromino block entries from the grid, preventing insertion blockage.
            for (BlockComponent const* block_component : tetromino.block_components)
            {
                RemoveBlockInGrid(a_playfield, block_component->grid_coordinate);
            }

            // Insert the blocks back into the grid at the new coordinates.
            for (int32 index(0); index < tetromino.blocks.size(); index++)
            {
                tetromino.block_components[index]->local_offset_coordinate = new_local_offset_coordinates[index];
                InsertBlockInGrid(a_playfield, tetromino.blocks[index], new_grid_coordinates[index], false);
            }
        }
    }

} // End of namespace ~ Tetriys.
