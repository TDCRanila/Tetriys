#include <Game/TetrominoComponent.h>

#include <Game/BlockComponent.h>
#include <Game/GameObjects.h>

#include <DFW/Defines/MathDefines.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace Tetriys
{
    TetrominoComponent::TetrominoComponent()
        : type(TetrominoType::None)
        , origin_block_index(0)
    {
    }

    void TetrominoComponent::ConstructTetromino(DFW::DECS::EntityRegistry& a_registry, TetrominoType const& a_tetromino_type)
    {       
        type = a_tetromino_type;

        auto SetupTetrominoBlock = [this, &a_registry](int32 const index, BlockCoordinate const& a_local_offset_coordinate, DFW::ColourRGBA const& a_tetromino_colour)
        {
            DFW::Entity& block = blocks[index];
            block = GameObjects::CreateBlockEntity(a_registry, a_tetromino_colour);
            block.SetParent(GetOwner());

            BlockComponent& block_component = block.GetComponent<BlockComponent>();
            block_components[index] = &block_component;
            block_component.local_offset_coordinate = a_local_offset_coordinate;
        };

        DFW::ColourRGBA const tetromino_colour = GetTetrominoColourFromType(type);

        switch (type)
        {
            case(TetrominoType::I):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0,  0), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate( 1,  0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 2,  0), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, -0), tetromino_colour);

                return;
            }
            case(TetrominoType::O):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate(0, 0), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate(0, 1), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate(1, 0), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(1, 1), tetromino_colour);

                return;
            }
            case(TetrominoType::J):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0,  0), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate( 1,  0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate(-1,  0), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1,  1), tetromino_colour);

                return;
            }
            case(TetrominoType::L):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0, 0), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate( 1, 0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 1, 1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, 0), tetromino_colour);

                return;
            }
            case(TetrominoType::T):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0, 0), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate( 1, 0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 0, 1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, 0), tetromino_colour);

                return;
            }
            case(TetrominoType::S):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0, 0), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate( 0, 1), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 1, 1), tetromino_colour);
                SetupTetrominoBlock(3, BlockCoordinate(-1, 0), tetromino_colour);

                return;
            }
            case(TetrominoType::Z):
            {
                // First Block - Origin.
                SetupTetrominoBlock(0, BlockCoordinate( 0,  0), tetromino_colour);
                SetupTetrominoBlock(2, BlockCoordinate( 0,  1), tetromino_colour);
                SetupTetrominoBlock(1, BlockCoordinate( 1,  0), tetromino_colour);
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

    DFW::ColourRGBA TetrominoComponent::GetTetrominoColourFromType(TetrominoType const& a_tetromino_type)
    {
        switch (a_tetromino_type)
        {
            case(TetrominoType::I):
            {
                return DFW::ColourRGBA::Cyan;
            }
            case(TetrominoType::O):
            {
                return DFW::ColourRGBA::Yellow;
            }
            case(TetrominoType::J):
            {
                return DFW::ColourRGBA::Blue;
            }
            case(TetrominoType::L):
            {
                return DFW::ColourRGBA::Orange;
            }
            case(TetrominoType::T):
            {
                return DFW::ColourRGBA::Purple;
            }
            case(TetrominoType::S):
            {
                return DFW::ColourRGBA::Green;
            }
            case(TetrominoType::Z):
            {
                return DFW::ColourRGBA::Red;
            }
            default:
            {
                DFW_ASSERT(false);
                return DFW::ColourRGBA();
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
