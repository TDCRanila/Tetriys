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

        //InsertBlockInGrid(_playfield, GameObjects::CreateBlockEntity(_ecs->Registry(), DFW::RandomColourRGBA()), BlockCoordinate(0, 19), false);
        //InsertBlockInGrid(_playfield, GameObjects::CreateBlockEntity(_ecs->Registry(), DFW::RandomColourRGBA()), BlockCoordinate(9, 19), false);

        //DFW::Entity T = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::T);
        //InsertTetromino(_playfield, T, BlockCoordinate(5, 19));

        //DFW::Entity I = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::I);
        //InsertTetromino(_playfield, I, BlockCoordinate(5, 15));
        //TranslateTetromino(_playfield, I, BlockCoordinate(-2, 0));
        //RotateTetromino(_playfield, I, TetrominoRotation::Clockwise);
        //TranslateTetromino(_playfield, I, BlockCoordinate(1, 1));

        //DFW::Entity O = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::O);
        //InsertTetromino(_playfield, O, BlockCoordinate(5, 11));
        //RotateTetromino(_playfield, O, TetrominoRotation::Clockwise180);

        //DFW::Entity J = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::J);
        //InsertTetromino(_playfield, J, BlockCoordinate(2, 10));

        //DFW::Entity L = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::L);
        //_ecs->SystemManager().GetSystem<GameControllerSystem>()->GetController<TetrominoController>("PlayerOne")->PossessTetromino(L);
        //L.AddComponent<TetrominoMovementComponent>();
        //L.AddComponent<GravityComponent>();
        //L.AddComponent<PlacementComponent>();
        //InsertTetromino(_playfield, L, BlockCoordinate(8, 10));

        //DFW::Entity Z = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::Z);
        //InsertTetromino(_playfield, Z, BlockCoordinate(2, 5));
        //RotateTetromino(_playfield, Z, TetrominoRotation::Clockwise180);

        //DFW::Entity S = GameObjects::CreateTetrominoEntity(_ecs->Registry(), TetrominoType::S);
        //InsertTetromino(_playfield, S, BlockCoordinate(8, 5));
        //MoveTetromino(_playfield, S, BlockCoordinate(8, 4));

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

        auto& play_director             = _ecs->SystemManager().AddSystem<PlayDirector>();
        auto& controller_system         = _ecs->SystemManager().AddSystem<GameControllerSystem>();
        controller_system.ExecuteAfter(play_director);
        auto& spawn_system = _ecs->SystemManager().AddSystem<SpawnSystem>();
        spawn_system.ExecuteAfter(controller_system);
        auto& playfield_control_system  = _ecs->SystemManager().AddSystem<PlayFieldControlSystem>();
        playfield_control_system.ExecuteAfter(controller_system);
        auto& tetromino_control_system  = _ecs->SystemManager().AddSystem<TetrominoControlSystem>();
        tetromino_control_system.ExecuteAfter(playfield_control_system);
        auto& gravity_system            = _ecs->SystemManager().AddSystem<GravitySystem>();
        gravity_system.ExecuteAfter(tetromino_control_system);
        auto& lock_system               = _ecs->SystemManager().AddSystem<LockTetrominoPlacementSystem>();
        lock_system.ExecuteAfter(gravity_system);


        auto& transform_system      = _ecs->SystemManager().AddSystem<DFW::TransformSystem>();
        transform_system.ExecuteAfter(lock_system);
        auto& camera_system         = _ecs->SystemManager().AddSystem<DFW::CameraSystem>();
        camera_system.ExecuteAfter(transform_system);
        auto& debug_render_system   = _ecs->SystemManager().AddSystem<DFW::DebugRenderSystem>();
        debug_render_system.ExecuteAfter(transform_system);
        auto& render_system         = _ecs->SystemManager().AddSystem<DFW::RenderSystem>();
        render_system.ExecuteAfter(transform_system);

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
