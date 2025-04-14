#pragma once

#include <DFW/Modules/ECS/Component.h>

#include <DFW/CoreSystems/TimeTracker.h>

namespace Tetriys
{
    struct GravityComponent final : DFW::DECS::Component::StrictRegistrar<GravityComponent>
    {
        DFW::TimeUnit max_time_between_fall_tick = 1.0f;
        DFW::TimeUnit current_between_fall_time = 0.0f;
    };

} // End of namespace ~ Tetriys
