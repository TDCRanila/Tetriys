#pragma once

#include <DFW/GameWorld/Controller/ControllerSystem.h>

namespace DFW
{
    namespace DECS
    {
        class EntityRegistry;
    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.

namespace Tetriys
{
    struct TetrominoPlacedEvent;
    struct TetrominoSpawnedEvent;

    class GameControllerSystem : public DFW::DECS::System::Registrar<GameControllerSystem, DFW::ControllerSystem>
    {
    public:
        GameControllerSystem() = default;
        virtual ~GameControllerSystem() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

    private:
        void OnTetrominoPlacedEvent(TetrominoPlacedEvent& a_event);
        void OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event);
    };

} // End of namespace ~ Tetriys.
