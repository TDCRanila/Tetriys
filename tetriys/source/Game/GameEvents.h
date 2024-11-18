#pragma once

#include <Game/GameState.h>

#include <DFW/Modules/ECS/Entity.h>

#include <DFW/CoreSystems/Events/EventImplementation.h>

namespace Tetriys
{
	enum class TetriysEvents
	{
		TetrominoPlacedEvent, TetrominoSpawnedEvent
	};

	class TetrominoPlacedEvent : public DFW::Event
	{
	public:
		TetrominoPlacedEvent(DFW::Entity& a_tetromino)
			: placed_tetromino(a_tetromino)
		{}

		DFW_CONSTRUCT_EVENT(TetriysEvents, TetrominoPlacedEvent);

		std::string GetDebugString() const override
		{
			std::stringstream debug_string;
			debug_string << GetName();
			debug_string << " - ";
			debug_string << "Tetromino Placed ";
			debug_string << "[";
			debug_string << placed_tetromino.GetID();
			debug_string << "]";

			return debug_string.str();
		}

		DFW::Entity placed_tetromino;
	};

	class TetrominoSpawnedEvent : public DFW::Event
	{
	public:
		TetrominoSpawnedEvent(DFW::Entity& a_tetromino, GameNameID const& a_spawned_in_game_id)
			: spawned_tetromino(a_tetromino)
			, spawned_in_game_id(a_spawned_in_game_id)
		{}

		DFW_CONSTRUCT_EVENT(TetriysEvents, TetrominoSpawnedEvent);

		DFW::Entity spawned_tetromino;
		GameNameID spawned_in_game_id;
	};

} // End of namespace ~ Tetriys.
