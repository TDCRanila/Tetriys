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
        QueueAction(new StrafeCommand(*_possessed_tetromino, a_move_direction));
    }

    void TetrominoController::Rotate(TetrominoRotation const& a_rotation)
    {
        QueueAction(new RotateCommand(*_possessed_tetromino, a_rotation));
    }

    void TetrominoController::SoftDrop()
    {
        QueueAction(new SoftDropCommand(*_possessed_tetromino));
    }

    void TetrominoController::HardDrop()
    {
        QueueAction(new HardDropCommand(*_possessed_tetromino));
    }

} // End of namespace ~ Tetriys.
