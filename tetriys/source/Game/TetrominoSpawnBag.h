#pragma once

#include <Game/TetrominoComponent.h>


#include <array>

namespace Tetriys
{
    struct TetrominoSpawnBag final : DFW::DECS::Component::StrictRegistrar<TetrominoSpawnBag>
    {
    public:
        TetrominoType const GetCurrentTetromino() const { return *bag_iterator; }
        void SelectNextTetromino() { bag_iterator++; }
        void SelectPreviousTetromino() { bag_iterator--; }
        bool IsAtLastTetrominoPiece() const { return tetromino_bag.end() == bag_iterator; }

        void PrintTetrominoList() const;

    public:
        std::array<TetrominoType, 7> tetromino_bag = { TetrominoType::None };
        std::array<TetrominoType, 7>::iterator bag_iterator;

        TetrominoType held_tetromino = TetrominoType::None;
        bool is_allowed_to_hold_tetromino = true;

    };

} // End of namespace ~ Tetriys.
