#pragma once

#include <DFW/GameWorld/Controller/ControllerSystem.h>

#include <DFW/Modules/ECS/Entity.h>

namespace DFW
{
    namespace DECS
    {
        class EntityRegistry;
    } // End of namespace ~ DECS.

} // End of namespace ~ DFW.

namespace Tetriys
{
    class GameControllerSystem : public DFW::DECS::System::Registrar<GameControllerSystem, DFW::ControllerSystem>
    {
    public:
        GameControllerSystem() = default;
        virtual ~GameControllerSystem() = default;

    private:
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

    };

} // End of namespace ~ Tetriys.
