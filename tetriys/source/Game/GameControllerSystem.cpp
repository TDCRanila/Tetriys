#include <Game/GameControllerSystem.h>

#include <Game/Control/TetrominoController.h>

#include <DFW/CoreSystems/Input/InputManagement.h>
#include <DFW/CoreSystems/CoreServices.h>

namespace Tetriys
{
    void GameControllerSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        auto input_system = DFW::CoreService::GetInputManagement();
        for (auto&& [name_id, controller] : _controllers)
        {
            DFW::SharedPtr<TetrominoController> const& tetromino_controller = std::dynamic_pointer_cast<TetrominoController>(controller);
            if (tetromino_controller && tetromino_controller->IsPossessing())
            {
                if (input_system->IsKeyPressed(DFW::DInput::DKey::A) || input_system->IsKeyRepeated(DFW::DInput::DKey::A))
                {
                    tetromino_controller->StrafeHorizontal(BlockCoordinate(-1, 0));
                }
                else if (input_system->IsKeyPressed(DFW::DInput::DKey::D) || input_system->IsKeyRepeated(DFW::DInput::DKey::D))
                {
                    tetromino_controller->StrafeHorizontal(BlockCoordinate(1, 0));
                }

                if (input_system->IsKeyPressed(DFW::DInput::DKey::W) || input_system->IsKeyRepeated(DFW::DInput::DKey::W))
                {
                    tetromino_controller->StrafeHorizontal(BlockCoordinate(0, 1));
                }
                else if (input_system->IsKeyPressed(DFW::DInput::DKey::S) || input_system->IsKeyRepeated(DFW::DInput::DKey::S))
                {
                    tetromino_controller->StrafeHorizontal(BlockCoordinate(0, -1));
                }

                if (input_system->IsKeyPressed(DFW::DInput::DKey::F))
                {
                    tetromino_controller->Rotate(TetrominoRotation::CounterClockwise);
                }
                else if (input_system->IsKeyPressed(DFW::DInput::DKey::G))
                {
                    tetromino_controller->Rotate(TetrominoRotation::Clockwise);
                }
                else if (input_system->IsKeyPressed(DFW::DInput::DKey::V))
                {
                    tetromino_controller->Rotate(TetrominoRotation::Clockwise180);
                }
            }
        }

        ControllerSystem::Update(a_registry);
    }

} // End of namespace ~ Tetriys.
