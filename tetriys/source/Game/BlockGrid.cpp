#include <Game/BlockGrid.h>

#include <DFW/Modules/ECS/ECSModule.h>

#include <Game/GameObjects.h>

namespace Tetriys
{
    void DataEntry::SyncBlockEntity()
    {
        DFW::TransformComponent& block_transform = block.GetComponent<DFW::TransformComponent>();
        block_transform.SetTranslation(block_world_start_position);

        BlockComponent& block_component = block.GetComponent<BlockComponent>();
        block_component.grid_coordinate = grid_coordinate;
    }

    void DataEntry::DestroyBlockEntity()
    {
        block.DestroySelf();
        block = DFW::Entity();
    }

    void BlockGrid::Setup(DFW::DECS::ECSModule& a_ecs)
    {
        entity_root = GameObjects::CreateGameObject(a_ecs);

        for (size_t index(0); index < block_grid_data.data.size(); index++)
        {
            DataEntry& data_entry = block_grid_data.data[index];
            data_entry.grid_coordinate = glm::ivec2(block_grid_data.GetXCoordinate(index), block_grid_data.GetYCoordinate(index));
            data_entry.block_world_start_position = glm::vec3(data_entry.grid_coordinate.x * TETRIYS_BLOCK_SPACING, data_entry.grid_coordinate.y * TETRIYS_BLOCK_SPACING, 0.0f);

            data_entry.block = GameObjects::CreateBlockEntity(a_ecs);
            data_entry.block.SetParent(entity_root);
                                   
            DFW::TransformComponent& transform_component = data_entry.block.GetComponent<DFW::TransformComponent>();
            transform_component.SetTranslation(data_entry.block_world_start_position);

        }
    }

    void BlockGrid::InsertBlockInGrid(DFW::Entity a_block, BlockCoordinate const a_coordinate, bool const a_override_block)
    {
        DataEntry& data_entry = block_grid_data.Get(a_coordinate.x, a_coordinate.y);

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

    void BlockGrid::RemoveBlockInGrid(DFW::Entity a_block)
    {
        RemoveBlockInGrid(a_block.GetComponent<BlockComponent>().grid_coordinate);
    }

    void BlockGrid::RemoveBlockInGrid(BlockCoordinate const a_coordinate)
    {
        block_grid_data.Get(a_coordinate.x, a_coordinate.y).DestroyBlockEntity();
    }

    void BlockGrid::MoveBlockInGrid(DFW::Entity a_block, BlockCoordinate const a_new_coordinate, bool a_override_block)
    {
        MoveBlockInGrid(a_block.GetComponent<BlockComponent>().grid_coordinate, a_new_coordinate, a_override_block);
    }

    void BlockGrid::MoveBlockInGrid(BlockCoordinate const a_current_coordinate, BlockCoordinate const a_new_coordinate, bool a_override_block)
    {
        DataEntry& data_entry = block_grid_data.Get(a_current_coordinate.x, a_current_coordinate.y);
        
        if (!data_entry.block.IsEntityValid())
            return; // No valid block to move to new coordinate.

        DataEntry& data_entry_at_new_coordinate = block_grid_data.Get(a_new_coordinate.x, a_new_coordinate.y);

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
