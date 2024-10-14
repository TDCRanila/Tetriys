#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/GameWorld/Transform.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Utility/Grid/Grid2D.h>

namespace Tetriys
{
    constexpr size_t TETRIYS_GRID_WIDTH = 10;
    constexpr size_t TETRIYS_GRID_HEIGHT = 40;
    constexpr float32 TETRIYS_BLOCK_SPACING = 2;

    struct DataEntry
    {
        void SyncBlockEntity();
        void DestroyBlockEntity();

        glm::vec3 block_world_start_position;
        BlockCoordinate grid_coordinate { 0, 0 };
        DFW::Entity block;

    };

    class BlockGrid
    {
    public:
        void Setup(DFW::DECS::ECSModule& a_ecs);

        void InsertBlockInGrid(DFW::Entity a_block, BlockCoordinate const a_coordinate, bool a_override_block);

        void RemoveBlockInGrid(DFW::Entity a_block);
        void RemoveBlockInGrid(BlockCoordinate const a_coordinate);

        void MoveBlockInGrid(DFW::Entity a_block, BlockCoordinate const a_new_coordinate, bool a_override_block);
        void MoveBlockInGrid(BlockCoordinate const a_current_coordinate, BlockCoordinate const a_new_coordinate, bool a_override_block);
        
        DFW::DUtility::StaticGrid2D<DataEntry, TETRIYS_GRID_WIDTH, TETRIYS_GRID_HEIGHT> block_grid_data;
        DFW::Entity entity_root;

    };

} // End of namespace ~ Tetriys.
