#pragma once

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Utility/Grid/Grid2D.h>

#include <Game/GameObjects.h>

namespace Tetriys
{
    constexpr size_t TETRIYS_GRID_WIDTH = 10;
    constexpr size_t TETRIYS_GRID_HEIGHT = 40;
    constexpr size_t TETRIYS_BLOCK_SPACING = 2;

    struct BlockGrid
    {
        void Setup(DFW::DECS::ECSModule& a_ecs);

        DFW::DUtility::StaticGrid2D<DFW::Entity, TETRIYS_GRID_WIDTH, TETRIYS_GRID_HEIGHT> blocks;
        DFW::Entity block_grid_root;
    };

} // End of namespace ~ Tetriys.
