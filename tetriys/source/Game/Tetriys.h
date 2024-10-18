#pragma once

#include <DFW/CoreSystems/Stage/Stage.h>

#include <DFW/Modules/ECS/ECSModule.h>

#include <Game/PlayField.h>

namespace Tetriys
{
	class TetriysGame final : public DFW::StageBase
	{
	public:
        TetriysGame(std::string const& a_stage_name, bool a_start_disabled = false);
        virtual ~TetriysGame() = default;

        virtual void OnUpdate() override;

        virtual void OnAttached() override;
        virtual void OnRemoved() override;

    private:
        PlayField _playfield;
        DFW::Entity _scrolling_block;
        
        DFW::Entity _camera_entity;

        void SetupECS();
        
        DFW::UniquePtr<DFW::DECS::ECSModule> _ecs;	
        
    private:
        void Debug_CreateXYZAxisOrigin();
        
        DFW::Entity _debug_xyz;

	};

} // End of namespace ~ Tetriys.
