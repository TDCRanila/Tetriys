#pragma once

#include <Game/BlockComponent.h>

#include <DFW/GameWorld/TransformComponent.h>
#include <DFW/GameWorld/GameObjectTagComponent.h>
#include <DFW/GameWorld/Graphics/ModelComponent.h>

#include <DFW/Modules/ECS/ECSModule.h>
#include <DFW/Modules/ECS/Entity.h>

namespace Tetriys
{
    namespace GameObjects
    {
        inline DFW::Entity CreateGameObject(DFW::DECS::ECSModule& a_ecs)
        {
            DFW::Entity game_object = a_ecs.Registry().CreateEntity();

            game_object.AddComponent<DFW::GameObjectTagComponent>();
            game_object.AddComponent<DFW::TransformComponent>();

            return game_object;
        }

        inline DFW::Entity CreateBlockEntity(DFW::DECS::ECSModule& a_ecs)
        {
            DFW::Entity block = a_ecs.Registry().CreateEntity();
            block.SetType<"Tetriys::Block">();

            block.AddComponent<DFW::GameObjectTagComponent>();
            block.AddComponent<DFW::TransformComponent>();
            block.AddComponent<BlockComponent>();
            auto& model_component = block.AddComponent<DFW::ModelComponent>();
            model_component.mesh = DFW::Debug_CreateBasicCube(DFW::RandomColourRGBA());

            return block;
        }

    } // End of namespace ~ GameObjects.

} // End of namespace ~ Tetriys.
