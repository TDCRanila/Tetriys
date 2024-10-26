#pragma once

#include <Game/Control/TetrominoRotation.h>
#include <Game/Control/PlayFieldActions.h>

#include <DFW/Modules/ECS/Component.h>

#include <glm/glm.hpp>

namespace Tetriys
{
    struct TetrominoMovementComponent final : public DFW::DECS::Component::StrictRegistrar<TetrominoMovementComponent>
    {
        TetrominoMovementComponent() : 
            desired_rotation_action(TetrominoRotation::None)
            , wants_to_soft_drop(false)
            , wants_to_hard_drop(false)
            , is_strafing(false)
            , is_rotating(false)
            , is_dropping(false)
        { }

        void ResetActions() 
        { 
            desired_movement_action = MovementAction();
            desired_rotation_action = TetrominoRotation::None;
            wants_to_soft_drop = false;
            wants_to_hard_drop = false;
        }

        MovementAction desired_movement_action;
        TetrominoRotation desired_rotation_action;
        bool wants_to_soft_drop;
        bool wants_to_hard_drop;
        
        bool is_strafing;
        bool is_rotating;
        bool is_dropping;
    };

} // End of namespace ~ Tetriys.
