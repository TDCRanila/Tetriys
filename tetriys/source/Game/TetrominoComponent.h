#pragma once

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Component.h>

#include <array>

namespace Tetriys
{
    struct BlockComponent;

    enum class TetrominoType
    {
        None, I, O, J, L, T, S, Z
    };

    struct TetrominoComponent final : public DFW::DECS::Component::StrictRegistrar<TetrominoComponent>
    {
        TetrominoComponent();

        void ConstructTetromino(DFW::DECS::ECSModule& a_ecs, TetrominoType const& a_tetromino_type);

        void Debug_DisplayBlockCoordinates() const;

        TetrominoType type;
        std::array<DFW::Entity, 4> blocks;
        std::array<BlockComponent*, 4> block_components { nullptr };
        int32 const origin_block_index; // Block Index of the block that is the origin point of the tetromino.

    };


} // End of namespace ~ Tetriys.
