#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/Modules/ECS/Component.h>

#include <DFW/Utility/ColourUtility.h>

namespace Tetriys
{
    struct BlockComponent final : DFW::DECS::Component::StrictRegistrar<BlockComponent>
    {
        BlockCoordinate grid_coordinate;
    };


} // End of namespace ~ Tetriys.
