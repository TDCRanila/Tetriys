#include <Game/Tetriys.h>

#include <Game/GameObjects.h>

#include <DFW/GameWorld/Camera/CameraSystem.h>
#include <DFW/GameWorld/Camera/CameraComponent.h>
#include <DFW/GameWorld/Graphics/RenderSystem.h>
#include <DFW/GameWorld/Graphics/DebugRenderSystem.h>
#include <DFW/GameWorld/TransformSystem.h>

#include <DFW/CoreSystems/Logging/Logger.h>
#include <DFW/CoreSystems/Input/InputManagement.h>

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
        auto input_system = DFW::CoreService::GetInputManagement();
        if (input_system->IsKeyPressed(DFW::DInput::DKey::A) || input_system->IsKeyRepeated(DFW::DInput::DKey::A))
        {
            _playfield.TranslateTetromino(_possessed_block, BlockCoordinate(-1, 0));
        }
        else if (input_system->IsKeyPressed(DFW::DInput::DKey::D) || input_system->IsKeyRepeated(DFW::DInput::DKey::D))
        {
            _playfield.TranslateTetromino(_possessed_block, BlockCoordinate(1, 0));
        }

        if (input_system->IsKeyPressed(DFW::DInput::DKey::W) || input_system->IsKeyRepeated(DFW::DInput::DKey::W))
        {
            _playfield.TranslateTetromino(_possessed_block, BlockCoordinate(0, 1));
        } 
        else if (input_system->IsKeyPressed(DFW::DInput::DKey::S) || input_system->IsKeyRepeated(DFW::DInput::DKey::S))
        {
            _playfield.TranslateTetromino(_possessed_block, BlockCoordinate(0, -1));
        }

        if (input_system->IsKeyPressed(DFW::DInput::DKey::F))
        {
            _playfield.RotateTetromino(_possessed_block, TetrominoRotation::CounterClockwise);
        }
        else if (input_system->IsKeyPressed(DFW::DInput::DKey::G))
        {
            _playfield.RotateTetromino(_possessed_block, TetrominoRotation::Clockwise);
        }
        else if (input_system->IsKeyPressed(DFW::DInput::DKey::V))
        {
            _playfield.RotateTetromino(_possessed_block, TetrominoRotation::Clockwise180);
        }

        if (input_system->IsKeyPressed(DFW::DInput::DKey::SPACE))
        {
            _possessed_block.DestroySelf();
        }

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

        _playfield.Setup(*_ecs);      

        DFW::Entity T = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::T);
        _playfield.InsertTetromino(T, BlockCoordinate(5, 19));
        //_playfield.RotateTetromino(T, TetrominoRotation::Clockwise);
        //_playfield.RotateTetromino(T, TetrominoRotation::Clockwise);
        //_playfield.TranslateTetromino(T, BlockCoordinate(2, 0));

        DFW::Entity I = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::I);
        _playfield.InsertTetromino(I, BlockCoordinate(5, 15));
        _playfield.TranslateTetromino(I, BlockCoordinate(-2, 0));
        _playfield.RotateTetromino(I, TetrominoRotation::Clockwise);
        _playfield.TranslateTetromino(I, BlockCoordinate(1, 1));

        DFW::Entity O = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::O);
        _playfield.InsertTetromino(O, BlockCoordinate(5, 11));
        _playfield.RotateTetromino(O, TetrominoRotation::Clockwise180);

        DFW::Entity J = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::J);
        _playfield.InsertTetromino(J, BlockCoordinate(2, 10));

        DFW::Entity L = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::L);
        _possessed_block = L;
        _playfield.InsertTetromino(L, BlockCoordinate(8, 10));

        DFW::Entity Z = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::Z);
        _playfield.InsertTetromino(Z, BlockCoordinate(2, 5));
        _playfield.RotateTetromino(Z, TetrominoRotation::Clockwise180);

        DFW::Entity S = GameObjects::CreateTetrominoEntity(*_ecs, TetrominoType::S);
        _playfield.InsertTetromino(S, BlockCoordinate(8, 5));
        _playfield.MoveTetromino(S, BlockCoordinate(8, 4));

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

        debug_render_system.ExecuteAfter(transform_system);
        render_system.ExecuteAfter(transform_system);
        transform_system.ExecuteAfter(camera_system);

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
