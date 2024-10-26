#pragma once

#include <Game/PlayField.h>

#include <DFW/Modules/ECS/System.h>

#include <DFW/CoreSystems/GameClock.h>

namespace Tetriys
{
    class GameLogicSystem : public DFW::DECS::System::Registrar<GameLogicSystem>
    {
    public:
        GameLogicSystem() = default;
        virtual ~GameLogicSystem() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

        void ExecuteGravity(DFW::DECS::EntityRegistry& a_registry);

    private:
        DFW::SharedPtr<DFW::GameClock> _game_clock;

    };


} // End of namespace ~ Tetriys.
