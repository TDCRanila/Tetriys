#include <Game/Tetriys.h>

#include <Game/GameObjects.h>
#include <Game/GameLogicSystem.h>
#include <Game/GravityComponent.h>
#include <Game/PlacementComponent.h>
#include <Game/GameControllerSystem.h>
#include <Game/Control/TetrominoRotation.h>
#include <Game/Control/PlayFieldControlSystem.h>
#include <Game/Control/TetrominoControlSystem.h>

#include <DFW/GameWorld/Camera/CameraSystem.h>
#include <DFW/GameWorld/Camera/CameraComponent.h>
#include <DFW/GameWorld/Graphics/RenderSystem.h>
#include <DFW/GameWorld/Graphics/DebugRenderSystem.h>
#include <DFW/GameWorld/TransformSystem.h>

#include <DFW/CoreSystems/Logging/Logger.h>

#include <DFW/Modules/ECS/Managers/SystemManager.h>
#include <DFW/Modules/Resource/Mesh/MeshLoader.h>

#include <DFW/Utility/FilePath.h>

namespace Tetriys
{
    TetriysGame::TetriysGame(std::string const& a_stage_name, bool a_start_disabled)
        : DFW::StageBase(a_stage_name, a_start_disabled)
    {
    }

    void TetriysGame::OnUpdate()
    {

        _ecs->UpdateECS();
    }

    void TetriysGame::OnAttached()
    {
        SetupECS();

        Debug_CreateXYZAxisOrigin();

        // Camera
        {
            _camera_entity = _ecs->Registry().CreateEntity();
            _camera_entity.SetName("camera-main");
            _camera_entity.SetType<"Camera">();

            _camera_entity.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(TETRIYS_GRID_WIDTH, TETRIYS_GRID_HEIGHT, -10.0f)));

            DFW::CameraComponent& camera_component = _camera_entity.AddComponent<DFW::CameraComponent>();
            DFW::CameraSystem* camera_system = _ecs->SystemManager().GetSystem<DFW::CameraSystem>();
            camera_system->ChangeCameraProjOrthographic(camera_component, 160.f, 90.f, DFW::ClipSpace(-100.f, 100.f), true);
            camera_system->RegisterCamera(camera_component, "camera-main");
            camera_system->SetActiveCamera("camera-main");
            camera_system->EnableCameraControl(camera_component);
        }

        _player_game = GameObjects::CreateGameEntry(*_ecs, "PlayerOne");

        PlayField& _playfield = _player_game.GetComponent<PlayField>();

        auto playfield_control_system = _ecs->SystemManager().GetSystem<PlayFieldControlSystem>();
        playfield_control_system->InsertBlockInGrid(_playfield, GameObjects::CreateBlockEntity(*_ecs, DFW::RandomColourRGBA()), BlockCoordinate(0, 19), false);
        playfield_control_system->InsertBlockInGrid(_playfield, GameObjects::CreateBlockEntity(*_ecs, DFW::RandomColourRGBA()), BlockCoordinate(9, 19), false);

        auto tetromino_control_system = _ecs->SystemManager().GetSystem<TetrominoControlSystem>();

        DFW::Entity T = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::T);
        tetromino_control_system->InsertTetromino(_playfield, T, BlockCoordinate(5, 19));

        DFW::Entity I = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::I);
        tetromino_control_system->InsertTetromino(_playfield, I, BlockCoordinate(5, 15));
        tetromino_control_system->TranslateTetromino(_playfield, I, BlockCoordinate(-2, 0));
        tetromino_control_system->RotateTetromino(_playfield, I, TetrominoRotation::Clockwise);
        tetromino_control_system->TranslateTetromino(_playfield, I, BlockCoordinate(1, 1));

        DFW::Entity O = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::O);
        tetromino_control_system->InsertTetromino(_playfield, O, BlockCoordinate(5, 11));
        tetromino_control_system->RotateTetromino(_playfield, O, TetrominoRotation::Clockwise180);

        DFW::Entity J = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::J);
        tetromino_control_system->InsertTetromino(_playfield, J, BlockCoordinate(2, 10));

        DFW::Entity L = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::L);
        _ecs->SystemManager().GetSystem<GameControllerSystem>()->GetController<TetrominoController>("PlayerOne")->PossessTetromino(L);
        L.AddComponent<TetrominoMovementComponent>();
        L.AddComponent<GravityComponent>();
        L.AddComponent<PlacementComponent>();
        tetromino_control_system->InsertTetromino(_playfield, L, BlockCoordinate(8, 10));

        DFW::Entity Z = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::Z);
        tetromino_control_system->InsertTetromino(_playfield, Z, BlockCoordinate(2, 5));
        tetromino_control_system->RotateTetromino(_playfield, Z, TetrominoRotation::Clockwise180);

        DFW::Entity S = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::S);
        tetromino_control_system->InsertTetromino(_playfield, S, BlockCoordinate(8, 5));
        tetromino_control_system->MoveTetromino(_playfield, S, BlockCoordinate(8, 4));

    }

    void TetriysGame::OnRemoved()
    {
        _ecs->Terminate();
    }

    void TetriysGame::SetupECS()
    {
        // Allocate
        _ecs = DFW::MakeUnique<DFW::DECS::ECSModule>();

        // Init Systems
        _ecs->Init();

        auto& render_system = _ecs->SystemManager().AddSystem<DFW::RenderSystem>();
        auto& debug_render_system = _ecs->SystemManager().AddSystem<DFW::DebugRenderSystem>();
        auto& camera_system = _ecs->SystemManager().AddSystem<DFW::CameraSystem>();
        auto& transform_system = _ecs->SystemManager().AddSystem<DFW::TransformSystem>();

        auto& playfield_control_system = _ecs->SystemManager().AddSystem<PlayFieldControlSystem>();
        auto& tetromino_control_system = _ecs->SystemManager().AddSystem<TetrominoControlSystem>();
        
        auto& controller_system = _ecs->SystemManager().AddSystem<GameControllerSystem>();
        auto& game_logic_system = _ecs->SystemManager().AddSystem<GameLogicSystem>();

        debug_render_system.ExecuteAfter(transform_system);
        render_system.ExecuteAfter(transform_system);
        transform_system.ExecuteAfter(camera_system);
        
        camera_system.ExecuteAfter(tetromino_control_system);

        tetromino_control_system.ExecuteAfter(playfield_control_system);
        playfield_control_system.ExecuteAfter(game_logic_system);
        game_logic_system.ExecuteAfter(controller_system);

        _ecs->SystemManager().CalculateSystemDependencies();
    }

    void TetriysGame::Debug_CreateXYZAxisOrigin()
    {
        _debug_xyz = _ecs->Registry().CreateEntity();
        _debug_xyz.SetName("xyz-axis");

        _debug_xyz.AddComponent<DFW::TransformComponent>(DFW::Transform(glm::vec3(0.0f, 0.0f, 0.0f)));

        DFW::ModelComponent& xyz_model = _debug_xyz.AddComponent<DFW::ModelComponent>();
        DFW::FilePath filepath(std::string("models") + DIR_SLASH + "xyz" + DIR_SLASH + "xyz_10x10.glb");
        xyz_model.mesh = DFW::DResource::LoadMesh(filepath);
    }

} // End of namespace ~ DGame.
