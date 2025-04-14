#pragma once

#include <Game/GameEvents.h>

#include <DFW/Modules/ECS/System.h>

namespace Tetriys
{
    class Debug_PlayfieldDataVisualizerSystem : public DFW::DECS::System::Registrar<Debug_PlayfieldDataVisualizerSystem>
    {
    public:
        Debug_PlayfieldDataVisualizerSystem() = default;
        virtual ~Debug_PlayfieldDataVisualizerSystem() = default;

    private:
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

    };

    class Debug_TetrominoDebugColourSystem : public DFW::DECS::System::Registrar<Debug_TetrominoDebugColourSystem>
    {
    public:
        Debug_TetrominoDebugColourSystem() = default;
        virtual ~Debug_TetrominoDebugColourSystem() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DFW::DECS::EntityRegistry& a_registry) override;

        void OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event);

    };

} // End of namespace ~ Tetriys.
