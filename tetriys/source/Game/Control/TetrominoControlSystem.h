#pragma once

#include <Game/BlockCoordinate.h>
#include <Game/Control/PlayFieldControlSystem.h>
#include <Game/Control/TetrominoRotation.h>

#include <DFW/GameWorld/Transform.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/System.h>

namespace Tetriys
{
    struct PlayField;

    class TetrominoControlSystem : public DFW::DECS::System::Registrar<TetrominoControlSystem, PlayFieldControlSystem>
    {
    public:
        void InsertTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate);
        void DestroyTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino);
        void RemoveTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino);
        void MoveTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate);
        void TranslateTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate_offset);
        void RotateTetromino(PlayField& a_playfield, DFW::Entity& a_tetromino, TetrominoRotation const& a_rotation);

    private:
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

    };

} // End of namespace ~ Tetriys.
