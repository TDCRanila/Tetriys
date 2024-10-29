#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/System.h>

#include <DFW/CoreSystems/GameClock.h>

namespace Tetriys
{
    constexpr BlockCoordinate TETRIYS_GRAVITY_TICK(0, -1.0f);

    struct TetrominoComponent;
    struct PlayFieldRef;

    inline bool IsTetrominoBlockedAtCoordinate(TetrominoComponent const& a_tetromino, struct PlayField const& a_playfield, BlockCoordinate const& a_coordinate);
    inline bool IsTetrominoBlockedAtCoordinate(DFW::Entity const& a_tetromino, BlockCoordinate const& a_coordinate);
    inline BlockCoordinate SimulateTetrominoFallLocation(DFW::Entity const& a_tetromino);

    class GameLogicSystem : public DFW::DECS::System::Registrar<GameLogicSystem>
    {
    public:
        GameLogicSystem() = default;
        virtual ~GameLogicSystem() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

        void ExecuteGravity(DFW::DECS::EntityRegistry& a_registry);
        void ExecuteTetrominoPlacementChecks(DFW::DECS::EntityRegistry& a_registry);

    private:
        DFW::SharedPtr<DFW::GameClock> _game_clock;

    };


} // End of namespace ~ Tetriys.
