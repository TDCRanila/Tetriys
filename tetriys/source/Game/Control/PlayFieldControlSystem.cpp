#include <Game/Control/PlayFieldControlSystem.h>

#include <Game/BlockComponent.h>
#include <Game/PlayField.h>
#include <Game/Control/PlayfieldActions.h>

#include <DFW/GameWorld/TransformComponent.h>

#include <DFW/Modules/ECS/ECSModule.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace Tetriys
{
    void InsertBlockInGrid(PlayField& a_playfield, DFW::Entity const& a_block, BlockCoordinate const& a_coordinate, bool const a_override_block)
    {
        PlayFieldDataEntry& data_entry = a_playfield.GetDataEntry(a_coordinate);

        if (data_entry.block.IsEntityValid())
        {
            if (a_override_block)
                data_entry.DestroyBlockEntity();
            else
                return; // Can't override block, as a valid block in the way.
        }

        data_entry.block = a_block;
        data_entry.SyncBlockEntity();

        data_entry.block.AddComponent<PlayFieldRef>(a_playfield);
    }

    void DestroyBlockInGrid(PlayField& a_playfield, DFW::Entity& a_block)
    {
        DestroyBlockInGrid(a_playfield, a_block.GetComponent<BlockComponent>().grid_coordinate);
    }

    void DestroyBlockInGrid(PlayField& a_playfield, BlockCoordinate const& a_coordinate)
    {
        a_playfield.GetDataEntry(a_coordinate).DestroyBlockEntity();
    }

    void RemoveBlockInGrid(PlayField& a_playfield, DFW::Entity& a_block)
    {
        RemoveBlockInGrid(a_playfield, a_block.GetComponent<BlockComponent>().grid_coordinate);
    }

    void RemoveBlockInGrid(PlayField& a_playfield, BlockCoordinate const& a_coordinate)
    {
        a_playfield. GetDataEntry(a_coordinate).ClearBlockEntity();
    }

    void MoveBlockInGrid(PlayField& a_playfield, DFW::Entity& a_block, BlockCoordinate const& a_new_coordinate, bool const a_override_block)
    {
        MoveBlockInGrid(a_playfield, a_block.GetComponent<BlockComponent>().grid_coordinate, a_new_coordinate, a_override_block);
    }

    void MoveBlockInGrid(PlayField& a_playfield, BlockCoordinate const& a_current_coordinate, BlockCoordinate const& a_new_coordinate, bool const a_override_block)
    {
        PlayFieldDataEntry& data_entry = a_playfield.GetDataEntry(a_current_coordinate);

        if (!data_entry.block.IsEntityValid())
            return; // No valid block to move to new coordinate.

        PlayFieldDataEntry& data_entry_at_new_coordinate = a_playfield.GetDataEntry(a_new_coordinate);

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

    void PlayFieldControlSystem::PreUpdate(DFW::DECS::EntityRegistry& a_registry)
    {
        // Destroy Blocks
        for (auto&& [e, block_destroy_tag, playfield_reference] : a_registry.ENTT().view<BlockDestroyTag, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            DestroyBlockInGrid(playfield_reference.Get(), entity);
            entity.DeleteComponent<BlockDestroyTag>();
        }

        // Remove Blocks
        for (auto&& [e, block_remove_tag, playfield_reference] : a_registry.ENTT().view<BlockRemoveTag, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            RemoveBlockInGrid(playfield_reference.Get(), entity);
            entity.DeleteComponent<BlockRemoveTag>();
        }

        // Insert Blocks
        for (auto&& [e, tetromino_insertion, playfield_reference] : a_registry.ENTT().view<BlockInsertAction, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            InsertBlockInGrid(playfield_reference.Get(), entity, tetromino_insertion.coordinate, false);
            entity.DeleteComponent<BlockInsertAction>();
        }

        // Move Blocks
        for (auto&& [e, block_movement_comp, playfield_reference] : a_registry.ENTT().view<BlockMoveAction, PlayFieldRef>().each())
        {
            DFW::Entity entity(e, a_registry);
            MoveBlockInGrid(playfield_reference.Get(), entity, block_movement_comp.desired_movement_action.coordinate, false);
            entity.DeleteComponent<BlockMoveAction>();
        }
    }

} // End of namespace ~ Tetriys.
