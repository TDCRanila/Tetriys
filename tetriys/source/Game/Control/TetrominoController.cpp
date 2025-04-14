#include <Game/Control/TetrominoController.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace Tetriys
{
    TetrominoController::TetrominoController()
    {
    }

    void TetrominoController::PossessTetromino(DFW::Entity& a_entity)
    {
        if (!a_entity && a_entity.IsOfType<"Tetriys::Tetromino">())
        {
            DFW_WARNLOG("Attempting to possess Tetromino Entity with controller, but entity is invalid or not of type!");
            return;
        }

        _possessed_tetromino = a_entity;
        _possessed_tetromino.AddComponent<PossessedByController>(GetNameID());
    }

    void TetrominoController::ReleaseTetromino() 
    { 
        if (!_possessed_tetromino)
        {
            DFW_INFOLOG("Attempting to release Tetromino Entity with controller, but none were possessed!");
            return;
        }

        _possessed_tetromino.DeleteComponent<PossessedByController>();
        _possessed_tetromino = DFW::Entity();
    }

    void TetrominoController::StrafeHorizontal(glm::ivec2 const& a_move_direction)
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<StrafeCommand>(_possessed_tetromino.GetComponent<TetrominoMovementComponent>(), a_move_direction));
    }

    void TetrominoController::Rotate(TetrominoRotation const& a_rotation)
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<RotateCommand>(_possessed_tetromino.GetComponent<TetrominoMovementComponent>(), a_rotation));
    }

    void TetrominoController::SoftDrop()
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<SoftDropCommand>(_possessed_tetromino.GetComponent<TetrominoMovementComponent>()));
    }

    void TetrominoController::HardDrop()
    {
        if (_possessed_tetromino)
            QueueAction(DFW::MakeUnique<HardDropCommand>(_possessed_tetromino.GetComponent<TetrominoMovementComponent>()));
    }

} // End of namespace ~ Tetriys.
