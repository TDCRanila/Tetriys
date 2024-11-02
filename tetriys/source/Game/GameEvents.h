#pragma once

#include <DFW/Modules/ECS/Entity.h>

#include <DFW/CoreSystems/Events/EventImplementation.h>

namespace Tetriys
{
	enum class TetriysEvents
	{
		TetrominoPlacedEvent
	};

	class TetrominoPlacedEvent : public DFW::Event
	{
	public:
		TetrominoPlacedEvent(DFW::Entity& a_tetromino)
			: tetromino(a_tetromino)
		{}

		DFW_CONSTRUCT_EVENT(TetriysEvents, TetrominoPlacedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "Tetromino Placed ";
			debug_string << "[";
			debug_string << tetromino.GetID();
			debug_string << "]";

			return debug_string.str();
		}

		DFW::Entity tetromino;
	};


} // End of namespace ~ Tetriys.
