#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/Modules/ECS/Component.h>

namespace Tetriys
{
    struct BlockComponent final : DFW::DECS::Component::StrictRegistrar<BlockComponent>
    {
        BlockCoordinate grid_coordinate;
        BlockCoordinate local_offset_coordinate;
    };


} // End of namespace ~ Tetriys.
