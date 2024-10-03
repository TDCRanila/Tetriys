#include <Game/Tetriys.h>

#include <DFW/CoreSystems/CoreServices.h>

#include <DFW/Modules/Editor/Console/MainConsole.h>
#include <DFW/Modules/Editor/MainMenuBar.h>

namespace Tetriys
{
	void GameApplication::PreApplicationInit()
	{

	}

	void GameApplication::PostApplicationInit()
	{
		// Stages
		DFW::CoreService::GetAppStageController()->AttachStage<DFW::DEditor::MainMenuBar>("MainMenuBar", false);
		DFW::CoreService::GetAppStageController()->AttachStage<DFW::DEditor::MainConsole>("Console", false);

	}

} // End of namespace ~ DGame.
