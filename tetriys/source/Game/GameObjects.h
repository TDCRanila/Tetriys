#pragma once

#include <Game/PlayField.h>
#include <Game/GameState.h>
#include <Game/BlockComponent.h>
#include <Game/TetrominoComponent.h>
#include <Game/GameControllerSystem.h>
#include <Game/Control/TetrominoController.h>

#include <DFW/GameWorld/TransformComponent.h>
#include <DFW/GameWorld/GameObjectTagComponent.h>
#include <DFW/GameWorld/Graphics/ModelComponent.h>

#include <DFW/Modules/ECS/ECSModule.h>
#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Managers/SystemManager.h>

#include <DFW/Utility/ColourUtility.h>

namespace Tetriys
{
    namespace GameObjects
    {
        inline DFW::Entity CreateGameObject(DFW::DECS::EntityRegistry& a_registry)
        {
            DFW::Entity game_object = a_registry.CreateEntity();

            game_object.AddComponent<DFW::GameObjectTagComponent>();
            game_object.AddComponent<DFW::TransformComponent>();

            return game_object;
        }

        inline DFW::Entity CreateBlockEntity(DFW::DECS::EntityRegistry& a_registry, DFW::ColourRGBA const& a_block_colour)
        {
            DFW::Entity block = CreateGameObject(a_registry);
            block.SetType<"Tetriys::Block">();

            block.AddComponent<BlockComponent>();

            DFW::ModelComponent& model_component = block.AddComponent<DFW::ModelComponent>();
            model_component.mesh = DFW::Debug_CreateBasicCube(a_block_colour);

            return block;
        }

        inline DFW::Entity CreateTetrominoEntity(DFW::DECS::EntityRegistry& a_registry, TetrominoType const& a_tetromino_type)
        {
            DFW::Entity tetromino = CreateGameObject(a_registry);
            tetromino.SetType<"Tetriys::Tetromino">();

            tetromino.AddComponent<TetrominoComponent>().ConstructTetromino(a_registry, a_tetromino_type);

            return tetromino;
        }

        inline DFW::Entity CreateGameEntry(DFW::DECS::ECSModule& a_ecs, std::string const& a_game_id)
        {
            DFW::Entity player_game = CreateGameObject(a_ecs.Registry());
            player_game.SetType<"Tetriys::GameEntry">();
            player_game.SetName(a_game_id);

            // Playfield
            PlayField& playfield = player_game.AddComponent<PlayField>();
            playfield.Setup();

            // Controller
            a_ecs.SystemManager().GetSystem<GameControllerSystem>()->ConstructController<TetrominoController>(DFW::ControllerNameID(a_game_id));
            player_game.AddComponent<ControllerRef>(DFW::ControllerNameID(a_game_id));

            // Score Component

            // Game/Playstate
            player_game.AddComponent<GameStateComponent>().game_state = GameState::START_GAME;
            player_game.AddComponent<GameStateComponent>().play_state = PlayState::NONE;
            player_game.AddComponent<GameNameIDComponent>().game_id = a_game_id;

            return player_game;
        }

    } // End of namespace ~ GameObjects.

} // End of namespace ~ Tetriys.
