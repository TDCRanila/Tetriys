#pragma once

#include <Game/Control/TetrominoController.h>

#include <DFW/CoreSystems/Stage/Stage.h>

#include <DFW/Modules/ECS/ECSModule.h>

namespace DFW
{
    struct ControllerSystem;
}

namespace Tetriys
{
    struct BlockControlSystem;
    struct TetrominoControlSystem;

	class TetriysGame final : public DFW::StageBase
	{
	public:
        TetriysGame(std::string const& a_stage_name, bool a_start_disabled = false);
        virtual ~TetriysGame() = default;

        virtual void OnUpdate() override;

        virtual void OnAttached() override;
        virtual void OnRemoved() override;

    private:
        DFW::SharedPtr<TetrominoController> _player_controller;
        DFW::Entity _player_game;
        DFW::Entity _camera_entity;

        void SetupECS();
        DFW::UniquePtr<DFW::DECS::ECSModule> _ecs;	
        
    private:
        void Debug_CreateXYZAxisOrigin();
        DFW::Entity _debug_xyz;

	};

} // End of namespace ~ Tetriys.
