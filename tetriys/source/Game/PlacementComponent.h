#pragma once

#include <DFW/Modules/ECS/Component.h>

#include <DFW/Defines/TimeDefines.h>

namespace Tetriys   
{
    struct PlacementComponent final : DFW::DECS::Component::StrictRegistrar<PlacementComponent>
    {
        bool check_placement_time = false;
        DFW::TimeUnit max_placement_time = 0.5f;
        DFW::TimeUnit current_placement_time = 0.0f;

    };

} // End of namespace ~ Tetriys.
