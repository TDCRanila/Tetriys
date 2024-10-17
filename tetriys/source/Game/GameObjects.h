#pragma once

#include <Game/BlockComponent.h>
#include <Game/TetrominoComponent.h>

#include <DFW/GameWorld/TransformComponent.h>
#include <DFW/GameWorld/GameObjectTagComponent.h>
#include <DFW/GameWorld/Graphics/ModelComponent.h>

#include <DFW/Modules/ECS/ECSModule.h>
#include <DFW/Modules/ECS/Entity.h>

#include <DFW/Utility/ColourUtility.h>

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

        inline DFW::Entity CreateBlockEntity(DFW::DECS::ECSModule& a_ecs, DFW::ColourRGBA const& a_block_colour)
        {
            DFW::Entity block = a_ecs.Registry().CreateEntity();
            block.SetType<"Tetriys::Block">();

            block.AddComponent<DFW::GameObjectTagComponent>();
            block.AddComponent<DFW::TransformComponent>();
            block.AddComponent<BlockComponent>();
            auto& model_component = block.AddComponent<DFW::ModelComponent>();
            model_component.mesh = DFW::Debug_CreateBasicCube(a_block_colour);

            return block;
        }

        inline DFW::Entity CreateTetrominoEntity(DFW::DECS::ECSModule& a_ecs, TetrominoType const& a_tetromino_type)
        {
            DFW::Entity tetromino = a_ecs.Registry().CreateEntity();
            tetromino.SetType<"Tetriys::Tetromino">();

            tetromino.AddComponent<DFW::GameObjectTagComponent>();
            tetromino.AddComponent<DFW::TransformComponent>();
            tetromino.AddComponent<TetrominoComponent>().ConstructTetromino(a_ecs, a_tetromino_type);

            return tetromino;
        }

    } // End of namespace ~ GameObjects.

} // End of namespace ~ Tetriys.
