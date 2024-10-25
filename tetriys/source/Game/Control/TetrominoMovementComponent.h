#pragma once

#include <Game/Control/TetrominoRotation.h>
#include <Game/Control/PlayFieldActions.h>

#include <DFW/Modules/ECS/Component.h>

#include <glm/glm.hpp>

namespace Tetriys
{
    struct TetrominoMovementComponent final : public DFW::DECS::Component::StrictRegistrar<TetrominoMovementComponent>
    {
        TetrominoMovementComponent() : desired_rotation_action(TetrominoRotation::None) { }

        void Reset() 
        { 
            desired_movement_action = MovementAction();
            desired_rotation_action = TetrominoRotation::None;
        }

        MovementAction desired_movement_action;
        TetrominoRotation desired_rotation_action;

    };

} // End of namespace ~ Tetriys.
