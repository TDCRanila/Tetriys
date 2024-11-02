#include <Game/PlayField.h>

#include <Game/GameObjects.h>

#include <DFW/GameWorld/Transform.h>

#include <DFW/Modules/ECS/ECSModule.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace Tetriys
{
    void PlayFieldDataEntry::SyncBlockEntity()
    {
        DFW::TransformComponent& block_transform = block.GetComponent<DFW::TransformComponent>();
        block_transform.SetTranslation(block_world_position);

        BlockComponent& block_component = block.GetComponent<BlockComponent>();
        block_component.grid_coordinate = grid_coordinate;
    }

    void PlayFieldDataEntry::ClearBlockEntity()
    {
        block = DFW::Entity();
    }

    void PlayFieldDataEntry::DestroyBlockEntity()
    {
        block.DestroySelf();
        block = DFW::Entity();
    }

    void PlayField::Setup(DFW::DECS::ECSModule& a_ecs)
    {
        for (size_t index(0); index < data.size(); index++)
        {
            PlayFieldDataEntry& data_entry = data[index];
            data_entry.grid_coordinate = glm::ivec2(GetXCoordinate(index), GetYCoordinate(index));
            data_entry.block_world_position = glm::vec3(data_entry.grid_coordinate.x * TETRIYS_BLOCK_SPACING, data_entry.grid_coordinate.y * TETRIYS_BLOCK_SPACING, 0.0f);
        }
    }

} // End of namespace ~ Tetriys.
