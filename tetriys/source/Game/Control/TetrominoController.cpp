#include <Game/Control/TetrominoController.h>

namespace Tetriys
{
    TetrominoController::TetrominoController()
        : _possessed_tetromino(nullptr)
    {
    }

    void TetrominoController::PossessTetromino(DFW::Entity& a_entity)
    { 
        _possessed_tetromino = &a_entity.AddComponent<TetrominoMovementComponent>();
    }

    void TetrominoController::ReleaseTetromino() 
    { 
        _possessed_tetromino->GetOwner().DeleteComponent<TetrominoMovementComponent>();
        _possessed_tetromino = nullptr; 
    }

    void TetrominoController::StrafeHorizontal(glm::ivec2 const& a_move_direction)
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<StrafeCommand>(*_possessed_tetromino, a_move_direction));
    }

    void TetrominoController::Rotate(TetrominoRotation const& a_rotation)
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<RotateCommand>(*_possessed_tetromino, a_rotation));
    }

    void TetrominoController::SoftDrop()
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<SoftDropCommand>(*_possessed_tetromino));
    }

    void TetrominoController::HardDrop()
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<HardDropCommand>(*_possessed_tetromino));
    }

} // End of namespace ~ Tetriys.
