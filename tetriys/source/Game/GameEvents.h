#pragma once

#include <Game/GameState.h>

#include <DFW/Modules/ECS/Entity.h>

#include <DFW/CoreSystems/Events/EventImplementation.h>

namespace Tetriys
{
	enum class TetriysEvents
	{
		TetrominoPlacedEvent, TetrominoSpawnedEvent, TetrominoHeldEvent,
		PlayfieldLineClearedEvent
	};

	class TetrominoPlacedEvent : public DFW::Event
	{
	public:
		TetrominoPlacedEvent(DFW::Entity& a_tetromino)
			: placed_tetromino(a_tetromino)
		{}

		virtual ~TetrominoPlacedEvent() = default;

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

		virtual ~TetrominoSpawnedEvent() = default;

		DFW_CONSTRUCT_EVENT(TetriysEvents, TetrominoSpawnedEvent);

		DFW::Entity spawned_tetromino;
		GameNameID spawned_in_game_id;
	};

	class TetrominoHeldEvent : public DFW::Event
	{
	public:
		TetrominoHeldEvent(DFW::Entity& a_tetromino)
			: tetromino_to_be_held(a_tetromino)
		{
		}

		virtual ~TetrominoHeldEvent() = default;

		DFW_CONSTRUCT_EVENT(TetriysEvents, TetrominoHeldEvent);

		DFW::Entity tetromino_to_be_held;
	};

	class PlayfieldLineClearedEvent : public DFW::Event
	{
	public:
		PlayfieldLineClearedEvent() = default;
		virtual ~PlayfieldLineClearedEvent() = default;

		DFW_CONSTRUCT_EVENT(TetriysEvents, PlayfieldLineClearedEvent);


		std::vector<int32> cleared_line_indices;
		DFW::Entity game_entry;

	};

} // End of namespace ~ Tetriys.
