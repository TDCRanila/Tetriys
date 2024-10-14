#include <Game/Tetriys.h>

#include <DFW/CoreSystems/Logging/Logger.h>

#include <DFW/Modules/ECS/Managers/SystemManager.h>
#include <DFW/Modules/Resource/Mesh/MeshLoader.h>

#include <DFW/GameWorld/Camera/CameraSystem.h>
#include <DFW/GameWorld/Camera/CameraComponent.h>
#include <DFW/GameWorld/Graphics/RenderSystem.h>
#include <DFW/GameWorld/Graphics/DebugRenderSystem.h>
#include <DFW/GameWorld/TransformSystem.h>

#include <DFW/Utility/Grid/Grid2D.h>
#include <DFW/Utility/FilePath.h>

#include <Game/GameObjects.h>

namespace Tetriys
{
    TetriysGame::TetriysGame(std::string const& a_stage_name, bool a_start_disabled)
        : DFW::StageBase(a_stage_name, a_start_disabled)
    {
    }

    void TetriysGame::OnUpdate()
    {
        _ecs->UpdateECS();

        BlockCoordinate new_coord = _scrolling_block.GetComponent<BlockComponent>().grid_coordinate;
        if (new_coord.x + 1 >= _playfield.block_grid_data.GetWidth())
        {
            new_coord.x = 0;
            
            if (new_coord.y + 1 >= _playfield.block_grid_data.GetHeight())
                new_coord.y = 0;
            else
                new_coord.y++;
        }
        else
        {
            new_coord.x++;
        }

        _playfield.MoveBlockInGrid(_scrolling_block, new_coord, true);
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
        _scrolling_block = GameObjects::CreateBlockEntity(*_ecs);
        _playfield.InsertBlockInGrid(_scrolling_block, BlockCoordinate(0, 0), true);

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
