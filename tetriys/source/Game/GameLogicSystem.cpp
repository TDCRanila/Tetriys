#include <Game/GameLogicSystem.h>

#include <Game/Control/TetrominoMovementComponent.h>
#include <Game/Control/PlayfieldActions.h>
#include <Game/GravityComponent.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>

#include <DFW/CoreSystems/CoreServices.h>

namespace Tetriys
{
    void GameLogicSystem::Init(DFW::DECS::EntityRegistry& a_registry)
    {
        _game_clock = DFW::CoreService::GetGameClock();
    }

    void GameLogicSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        ExecuteGravity(a_registry);
    }

    void GameLogicSystem::ExecuteGravity(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, gravity_component, tetromino_movement_component] : a_registry.ENTT().view<GravityComponent, TetrominoMovementComponent>().each())
        {
            if (!tetromino_movement_component.is_dropping)
            {
                bool const apply_gravity_this_frame = gravity_component.current_between_fall_time >= gravity_component.max_time_betweem_fall_tick;
                if (apply_gravity_this_frame)
                {
                    tetromino_movement_component.desired_movement_action.coordinate = BlockCoordinate(0, -1);
                    gravity_component.current_between_fall_time = 0;
                }

                gravity_component.current_between_fall_time += _game_clock->GetLastFrameDeltaTime();
            }
            else
            {
                gravity_component.current_between_fall_time = 0;
            }
        }
    }


} // End of namespace ~ Tetriys.
