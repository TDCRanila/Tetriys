#include <Game/PlayField.h>

#include <Game/GameObjects.h>

#include <DFW/CoreSystems/Logging/Logger.h>

#include <DFW/Modules/ECS/ECSModule.h>

#include <functional>

namespace Tetriys
{
    void PlayFieldDataEntry::SyncBlockEntity()
    {
        DFW::TransformComponent& block_transform = block.GetComponent<DFW::TransformComponent>();
        block_transform.SetTranslation(block_world_position);

        BlockComponent& block_component = block.GetComponent<BlockComponent>();
        block_component.grid_coordinate = grid_coordinate;
    }

    void PlayFieldDataEntry::ClearBlockEntity()
    {
        block = DFW::Entity();
    }

    void PlayFieldDataEntry::DestroyBlockEntity()
    {
        block.DestroySelf();
        block = DFW::Entity();
    }

    void PlayField::InsertTetromino(DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        for (int32 index(0); index < tetromino.blocks.size(); index++)
            InsertBlockInGrid(tetromino.blocks[index], a_coordinate + tetromino.block_components[index]->local_offset_coordinate, false);
    }

    void PlayField::DestroyTetromino(DFW::Entity& a_tetromino)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        for (int32 index(0); index < tetromino.blocks.size(); index++)
            DestroyBlockInGrid(tetromino.block_components[index]->grid_coordinate);
    }

    void PlayField::RemoveTetromino(DFW::Entity& a_tetromino)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        for (int32 index(0); index < tetromino.blocks.size(); index++)
            RemoveBlockInGrid(tetromino.block_components[index]->grid_coordinate);
    }

    void PlayField::MoveTetromino(DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate)
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
            if (!IsValidGridCoordinate(coordinate))
            {
                can_tetromino_be_moved = false;
                break;
            }

            // Check for blocks other than the blocks of the tetromino.
            PlayFieldDataEntry const& data_entry = GetDataEntry(coordinate);
            if (data_entry.block.IsEntityValid() && !data_entry.block.IsChildOfEntity(a_tetromino))
            {
                if (!data_entry.block.IsChildOfEntity(a_tetromino))
                    can_tetromino_be_moved = false;
            }
        }

        if (can_tetromino_be_moved)
        {
            // Remove tetromino block entries from the grid, preventing insertion blockage.
            for (BlockComponent const* block_component : tetromino.block_components)
            {
                RemoveBlockInGrid(block_component->grid_coordinate);
            }

            // Insert the blocks back into the grid at the new coordinates.
            for (int32 index(0); index < tetromino.blocks.size(); index++)
            {
                InsertBlockInGrid(tetromino.blocks[index], new_grid_coordinates[index], false);
            }
        }
    }

    void PlayField::TranslateTetromino(DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate_offset)
    {
        TetrominoComponent const& tetromino = a_tetromino.GetComponent<TetrominoComponent>();
        MoveTetromino(a_tetromino, tetromino.block_components[tetromino.origin_block_index]->grid_coordinate + a_coordinate_offset);
    }

    void PlayField::RotateTetromino(DFW::Entity& a_tetromino, TetrominoRotation const& a_rotation)
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
            if (!IsValidGridCoordinate(coordinate))
            {
                can_tetromino_be_rotated = false;
                break;
            }

            // Check for blocks other than the blocks of the tetromino.
            PlayFieldDataEntry const& data_entry = GetDataEntry(coordinate);
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
                RemoveBlockInGrid(block_component->grid_coordinate);
            }

            // Insert the blocks back into the grid at the new coordinates.
            for (int32 index(0); index < tetromino.blocks.size(); index++)
            {
                tetromino.block_components[index]->local_offset_coordinate = new_local_offset_coordinates[index];
                InsertBlockInGrid(tetromino.blocks[index], new_grid_coordinates[index], false);
            }
        }
    }

    void PlayField::Setup(DFW::DECS::ECSModule& a_ecs)
    {
        entity_root = GameObjects::CreateGameObject(a_ecs);

        for (size_t index(0); index < data.size(); index++)
        {
            PlayFieldDataEntry& data_entry = data[index];
            data_entry.grid_coordinate = glm::ivec2(GetXCoordinate(index), GetYCoordinate(index));
            data_entry.block_world_position = glm::vec3(data_entry.grid_coordinate.x * TETRIYS_BLOCK_SPACING, data_entry.grid_coordinate.y * TETRIYS_BLOCK_SPACING, 0.0f);
        }
    }

    void PlayField::InsertBlockInGrid(DFW::Entity const& a_block, BlockCoordinate const& a_coordinate, bool const a_override_block)
    {
        PlayFieldDataEntry& data_entry = GetDataEntry(a_coordinate);

        if (data_entry.block.IsEntityValid())
        {
            if (a_override_block)
                data_entry.DestroyBlockEntity();
            else
                return; // Can't override block, as a valid block in the way.
        }

        data_entry.block = a_block;
        data_entry.SyncBlockEntity();
    }

    void PlayField::DestroyBlockInGrid(DFW::Entity& a_block)
    {
        DestroyBlockInGrid(a_block.GetComponent<BlockComponent>().grid_coordinate);
    }
    
    void PlayField::DestroyBlockInGrid(BlockCoordinate const& a_coordinate)
    {
        GetDataEntry(a_coordinate).DestroyBlockEntity();
    }

    void PlayField::RemoveBlockInGrid(DFW::Entity& a_block)
    {
        RemoveBlockInGrid(a_block.GetComponent<BlockComponent>().grid_coordinate);
    }

    void PlayField::RemoveBlockInGrid(BlockCoordinate const& a_coordinate)
    {
        GetDataEntry(a_coordinate).ClearBlockEntity();
    }

    void PlayField::MoveBlockInGrid(DFW::Entity& a_block, BlockCoordinate const& a_new_coordinate, bool const a_override_block)
    {
        MoveBlockInGrid(a_block.GetComponent<BlockComponent>().grid_coordinate, a_new_coordinate, a_override_block);
    }

    void PlayField::MoveBlockInGrid(BlockCoordinate const& a_current_coordinate, BlockCoordinate const& a_new_coordinate, bool const a_override_block)
    {
        PlayFieldDataEntry& data_entry = GetDataEntry(a_current_coordinate);
        
        if (!data_entry.block.IsEntityValid())
            return; // No valid block to move to new coordinate.

        PlayFieldDataEntry& data_entry_at_new_coordinate = GetDataEntry(a_new_coordinate);

        if (data_entry_at_new_coordinate.block.IsEntityValid())
        {
            if (a_override_block)
                data_entry_at_new_coordinate.DestroyBlockEntity();
            else
                return; // Can't override block, as a valid block in the way.
        }

        // Move block entity to new data_entry.
        data_entry_at_new_coordinate.block = data_entry.block;
        data_entry_at_new_coordinate.SyncBlockEntity();

        data_entry.block = DFW::Entity();
    }

} // End of namespace ~ Tetriys.
