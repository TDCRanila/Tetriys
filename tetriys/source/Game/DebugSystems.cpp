#include <Game/DebugSystems.h>

#include <Game/PlayField.h>
#include <Game/GameObjects.h>

#include <DFW/GameWorld/TransformComponent.h>
#include <DFW/GameWorld/Graphics/DebugRenderSystem.h>

#include <DFW/CoreSystems/Events/EventDispatcher.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Managers/EntityRegistry.h>
#include <DFW/Modules/ECS/Managers/SystemManager.h>

namespace Tetriys
{
    void Debug_PlayfieldDataVisualizerSystem::Update(DFW::DECS::EntityRegistry& a_registry)
    {
        for (auto&& [e, playfield] : a_registry.ENTT().view<PlayField const>().each())
        {
            float32 const z_offset(2.0f);
            for (PlayFieldDataEntry const& data_entry : playfield.data)
            {
                {
                    // Debug Render Data Entries.
                    glm::vec3 const& data_entry_world_position = data_entry.block_world_position;
                    DFW::Transform const debug_transform({ data_entry_world_position.x, data_entry_world_position.y, data_entry_world_position.z - z_offset });
                    SystemManager().GetSystem<DFW::DebugRenderSystem>()->DrawSphere(debug_transform
                        , 0.1f
                        , DFW::DebugDrawSettings(data_entry.block ? DFW::ColourRGBA::White : DFW::ColourRGBA::DarkRed, true));
                }

                // Debug Render Valid Blocks.
                if (data_entry.block)
                {
                    DFW::TransformComponent const& block_transform_comp = data_entry.block.GetComponent<DFW::TransformComponent>();

                    glm::vec3 const& block_world_position(block_transform_comp.GetWorldTranslation());
                    DFW::Transform const debug_transform({ block_world_position.x, block_world_position.y, block_world_position.z - z_offset });
                    SystemManager().GetSystem<DFW::DebugRenderSystem>()->DrawCube(debug_transform
                        , 0.3f
                        , DFW::DebugDrawSettings(DFW::ColourRGBA::Magenta, true));
                }
            }
        }
    }

    void Debug_TetrominoDebugColourSystem::Init(DFW::DECS::EntityRegistry& a_registry)
    {
        ECSEventHandler().RegisterCallback<TetrominoSpawnedEvent, &Debug_TetrominoDebugColourSystem::OnTetrominoSpawnedEvent>(this);
    }

    void Debug_TetrominoDebugColourSystem::Terminate(DFW::DECS::EntityRegistry& a_registry)
    {
        ECSEventHandler().UnregisterCallback<TetrominoSpawnedEvent, &Debug_TetrominoDebugColourSystem::OnTetrominoSpawnedEvent>(this);
    }

    void Debug_TetrominoDebugColourSystem::OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event)
    {
        if (DFW::Entity tetromino = a_event.spawned_tetromino)
        {
            // Create new Tetromino Block meshes with debug colouring.
            TetrominoComponent& tetromino_component = tetromino.GetComponent<TetrominoComponent>();

            DFW::ColourRGBA const tetromino_base_colour = TetrominoComponent::GetTetrominoColourFromType(tetromino_component.type);
            DFW::ColourRGBA const debug_colour0         = tetromino_base_colour.Mix(DFW::ColourRGBA::LightGrey);
            DFW::ColourRGBA const debug_colour1         = tetromino_base_colour.Mix(DFW::ColourRGBA::Black);
            DFW::ColourRGBA const debug_colour2         = tetromino_base_colour.Mix(DFW::ColourRGBA::DarkGrey);
            DFW::ColourRGBA const debug_colour3         = tetromino_base_colour.Mix(DFW::ColourRGBA::White);

            tetromino_component.blocks[0].GetComponent<DFW::ModelComponent>().mesh = DFW::Debug_CreateBasicCube(debug_colour0);
            tetromino_component.blocks[1].GetComponent<DFW::ModelComponent>().mesh = DFW::Debug_CreateBasicCube(debug_colour1);
            tetromino_component.blocks[2].GetComponent<DFW::ModelComponent>().mesh = DFW::Debug_CreateBasicCube(debug_colour2);
            tetromino_component.blocks[3].GetComponent<DFW::ModelComponent>().mesh = DFW::Debug_CreateBasicCube(debug_colour3);
        }
    }

} // End of namespace ~ Tetriys.
