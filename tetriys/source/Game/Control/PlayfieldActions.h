#pragma once

#include <Game/PlayField.h>

#include <DFW/Modules/ECS/Component.h>

#include <glm/glm.hpp>

namespace Tetriys
{
    struct BlockDestroyTag final : public DFW::DECS::Component::StrictRegistrar<BlockDestroyTag> {};
    struct BlockRemoveTag final : public DFW::DECS::Component::StrictRegistrar<BlockRemoveTag> {};

    struct MovementAction
    {
        MovementAction() : coordinate(0, 0), is_absolute_coordinate(false) { }
        
        BlockCoordinate coordinate;
        bool is_absolute_coordinate; 
    };

    struct BlockInsertAction final : public DFW::DECS::Component::StrictRegistrar<BlockInsertAction>
    {
        BlockInsertAction() = default;
        BlockInsertAction(BlockCoordinate& a_insertion_coordinate) : coordinate(a_insertion_coordinate) { }

        BlockCoordinate coordinate;

    };

    struct BlockMoveAction final : public DFW::DECS::Component::StrictRegistrar<BlockMoveAction>
    {
        void Reset() { desired_movement_action = MovementAction(); }

        MovementAction desired_movement_action;

    };

    struct TetrominoDestroyTag final : public DFW::DECS::Component::StrictRegistrar<TetrominoDestroyTag> {};
    struct TetrominoRemoveTag final : public DFW::DECS::Component::StrictRegistrar<TetrominoRemoveTag> {};

    struct TetrminoInsertAction final : public DFW::DECS::Component::StrictRegistrar<TetrminoInsertAction>
    {
        TetrminoInsertAction() = default;
        TetrminoInsertAction(BlockCoordinate const& a_insertion_coordinate) : coordinate(a_insertion_coordinate) { }

        BlockCoordinate coordinate;

    };

} // End of namespace ~ Tetriys.
