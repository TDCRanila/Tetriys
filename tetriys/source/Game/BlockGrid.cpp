#include <Game/BlockGrid.h>

#include <DFW/Modules/ECS/ECSModule.h>

namespace Tetriys
{
    void BlockGrid::Setup(DFW::DECS::ECSModule& a_ecs)
    {
        block_grid_root = GameObjects::CreateGameObject(a_ecs);

        for (size_t index(0); index < blocks.data.size(); index++)
        {
            blocks.data[index] = GameObjects::CreateBlockEntity(a_ecs);
            DFW::Entity& block = blocks.data[index];

            block.SetParent(block_grid_root);

            DFW::TransformComponent& transform_component = block.GetComponent<DFW::TransformComponent>();
            transform_component.SetTranslation(glm::vec3(blocks.GetXCoordinate(index) * TETRIYS_BLOCK_SPACING, blocks.GetYCoordinate(index) * TETRIYS_BLOCK_SPACING, 0.0f));
        }
    }

} // End of namespace ~ Tetriys.
