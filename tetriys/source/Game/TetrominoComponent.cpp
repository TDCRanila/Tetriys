#include <Game/TetrominoComponent.h>

#include <Game/BlockComponent.h>
#include <Game/GameObjects.h>

#include <DFW/Utility/ColourUtility.h>

#include <DFW/Defines/MathDefines.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace Tetriys
{
    TetrominoComponent::TetrominoComponent()
        : type(TetrominoType::None)
        , origin_block_index(0)
    {
    }

    void TetrominoComponent::ConstructTetromino(DFW::DECS::ECSModule& a_ecs, TetrominoType const& a_tetromino_type)
    {       
        type = a_tetromino_type;

        auto SetupTetrominoBlock = [this, &a_ecs](int32 const index, BlockCoordinate const& a_local_offset_coordinate, DFW::ColourRGBA const& a_tetromino_colour)
        {
            DFW::Entity& block = blocks[index];
            blocks[index] = GameObjects::CreateBlockEntity(a_ecs, a_tetromino_colour);
            block.SetParent(GetOwner());

            BlockComponent& block_component = block.GetComponent<BlockComponent>();
            block_components[index] = &block_component;
            block_component.local_offset_coordinate = a_local_offset_coordinate;
        };

        DFW::ColourRGBA const tetromino_origin_colour = DFW::ColourRGBA::LightGrey;
        switch (type)
        {
            case(TetrominoType::I):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Cyan;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0,  0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate( 1,  0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 2,  0), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, -0), tetromino_colour);

                return;
            }
            case(TetrominoType::O):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Yellow;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate(0, 0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate(0, 1), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate(1, 0), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(1, 1), tetromino_colour);

                return;
            }
            case(TetrominoType::J):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Blue;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0,  0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate( 1,  0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate(-1,  0), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1,  1), tetromino_colour);

                return;
            }
            case(TetrominoType::L):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Orange;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0, 0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate( 1, 0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 1, 1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, 0), tetromino_colour);

                return;
            }
            case(TetrominoType::T):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Purple;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0, 0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate( 1, 0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 0, 1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, 0), tetromino_colour);

                return;
            }
            case(TetrominoType::S):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Green;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0, 0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate( 0, 1), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 1, 1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, 0), tetromino_colour);

                return;
            }
            case(TetrominoType::Z):
            {
                DFW::ColourRGBA const tetromino_colour = DFW::ColourRGBA::Red;

                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0,  0), tetromino_colour.Mix(tetromino_origin_colour));
                SetupTetrominoBlock(1, BlockCoordinate( 1,  0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 0,  1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1,  1), tetromino_colour);

                return;
            }
            default:
            {
                DFW_ASSERT(false);
                return;
            }
        }
    }

    void TetrominoComponent::Debug_DisplayBlockCoordinates() const
    {
        DFW_INFOLOG("Tetromino/ComponentID: {} - TetrominoType: {}", GetID(), static_cast<int32>(type));
        for (BlockComponent const* block_component : block_components)
        {
            DFW_INFOLOG("Grid-XY: {}-{}, Offset-XY: {}-{}",
                block_component->grid_coordinate.x,
                block_component->grid_coordinate.y,
                block_component->local_offset_coordinate.x,
                block_component->local_offset_coordinate.y
            );
        }
    }

} // End of namespace ~ Tetriys.
