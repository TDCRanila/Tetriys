#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/GameWorld/Transform.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Utility/Grid/StaticGrid2D.h>

namespace Tetriys
{
    constexpr size_t TETRIYS_GRID_WIDTH = 10;
    constexpr size_t TETRIYS_GRID_HEIGHT = 40;
    constexpr float32 TETRIYS_BLOCK_SPACING = 2;

    enum class TetrominoRotation
    {
        None,
        Clockwise,
        CounterClockwise,
        Clockwise180
    };

    struct PlayFieldDataEntry
    {
        void SyncBlockEntity();
        void ClearBlockEntity();
        void DestroyBlockEntity();

        glm::vec3 block_world_position;
        BlockCoordinate grid_coordinate{ 0, 0 };
        DFW::Entity block;
    };

    class PlayField : public DFW::DUtility::StaticGrid2D<PlayFieldDataEntry, TETRIYS_GRID_WIDTH, TETRIYS_GRID_HEIGHT>
    {
    public:
        void Setup(DFW::DECS::ECSModule& a_ecs);
        DFW::Entity const& GetRoot() { return entity_root; }

    public:
        void InsertTetromino(DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate);
        void DestroyTetromino(DFW::Entity& a_tetromino);
        void RemoveTetromino(DFW::Entity& a_tetromino);
        void MoveTetromino(DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate);
        void TranslateTetromino(DFW::Entity& a_tetromino, BlockCoordinate const& a_coordinate_offset);
        void RotateTetromino(DFW::Entity& a_tetromino, TetrominoRotation const& a_rotation);
    
    public:
        void InsertBlockInGrid(DFW::Entity const& a_block, BlockCoordinate const& a_coordinate, bool const a_override_block);

        void DestroyBlockInGrid(DFW::Entity& a_block);
        void DestroyBlockInGrid(BlockCoordinate const& a_coordinate);

        void RemoveBlockInGrid(DFW::Entity& a_block);
        void RemoveBlockInGrid(BlockCoordinate const& a_coordinate);

        void MoveBlockInGrid(DFW::Entity& a_block, BlockCoordinate const& a_new_coordinate, bool const a_override_block);
        void MoveBlockInGrid(BlockCoordinate const& a_current_coordinate, BlockCoordinate const& a_new_coordinate, bool const a_override_block);
        
        bool IsValidGridCoordinate(BlockCoordinate const& a_coordinate) const { return IsValidCoordinate(a_coordinate.x, a_coordinate.y); }

    private:
        PlayFieldDataEntry const& GetDataEntry(BlockCoordinate const& a_coordinate) const { return Get(a_coordinate.x, a_coordinate.y); }
        PlayFieldDataEntry& GetDataEntry(BlockCoordinate const& a_coordinate) { return Get(a_coordinate.x, a_coordinate.y); }

    private:
        DFW::Entity entity_root;

    };

} // End of namespace ~ Tetriys.
