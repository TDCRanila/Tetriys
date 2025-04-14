#pragma once

#include <Game/Control/TetrominoMovementComponent.h>
#include <Game/Control/TetrominoRotation.h>

#include <DFW/GameWorld/Controller/BaseController.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Component.h>

#include <DFW/CoreSystems/Command.h>

#include <glm/glm.hpp>

namespace Tetriys
{
    struct ControllerRef final : public DFW::DECS::Component::StrictRegistrar<ControllerRef>
    {
        ControllerRef() = default;
        ControllerRef(DFW::ControllerNameID const& a_controller_name_id) : controller_name_id(a_controller_name_id) {}

        DFW::ControllerNameID const controller_name_id;
    };

    struct PossessedByController final : public DFW::DECS::Component::StrictRegistrar<PossessedByController>
    {
        PossessedByController() = default;
        PossessedByController(DFW::ControllerNameID const& a_controller_name_id) : controller_name_id(a_controller_name_id) {}

        DFW::ControllerNameID const controller_name_id;
    };

    class TetrominoController : public DFW::BaseController
    {
    public:
        TetrominoController();
        virtual ~TetrominoController() = default;
        
        void PossessTetromino(DFW::Entity& a_entity);
        void ReleaseTetromino();

        DFW::Entity& GetPossessedTetromino() { return _possessed_tetromino; }
    
    public:
        void StrafeHorizontal(glm::ivec2 const& a_move_direction);
        void Rotate(TetrominoRotation const& a_rotation);

        void SoftDrop();
        void HardDrop();

        void HoldTetromino();

    private:
        DFW::Entity _possessed_tetromino;

    private:
        class StrafeCommand : public DFW::Command
        {
        public:
            StrafeCommand(TetrominoMovementComponent& a_tetromino, glm::ivec2 const& a_move_direction) : tetromino(a_tetromino), move_direction(a_move_direction) {}

            virtual void Execute() override
            {
                tetromino.get().desired_movement_action.coordinate += move_direction;
            }

            DFW::RefWrap<TetrominoMovementComponent> tetromino;
            glm::ivec2 move_direction;
        };

        class RotateCommand : public DFW::Command
        {
        public:
            RotateCommand(TetrominoMovementComponent& a_tetromino, TetrominoRotation const& a_rotation) : tetromino(a_tetromino), rotation_input(a_rotation) {}

            virtual void Execute() override
            {
                tetromino.get().desired_rotation_action = rotation_input;
            }

            DFW::RefWrap<TetrominoMovementComponent> tetromino;
            TetrominoRotation rotation_input;
        };

        class SoftDropCommand : public DFW::Command
        {
        public:
            SoftDropCommand(TetrominoMovementComponent& a_tetromino) : tetromino(a_tetromino) {}

            virtual void Execute() override
            {
                tetromino.get().wants_to_soft_drop = true;
            }

            DFW::RefWrap<TetrominoMovementComponent> tetromino;
        };

        class HardDropCommand : public DFW::Command
        {
        public:
            HardDropCommand(TetrominoMovementComponent& a_tetromino) : tetromino(a_tetromino) {}

            virtual void Execute() override
            {
                tetromino.get().wants_to_hard_drop = true;
            }

            DFW::RefWrap<TetrominoMovementComponent> tetromino;
        };

    };


} // End of namespace ~ Tetriys.
