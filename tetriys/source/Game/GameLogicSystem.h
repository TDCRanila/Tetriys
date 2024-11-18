#pragma once

#include <Game/GameEvents.h>
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
    inline bool IsTetrominoBlockedNextGravityTick(TetrominoComponent const& a_tetromino);
    
    class PlayDirector : public DFW::DECS::System::Registrar<PlayDirector>
    {
    public:
        PlayDirector() = default;
        virtual ~PlayDirector() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

        void OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event);
        void OnTetrominoPlacedEvent(TetrominoPlacedEvent& a_event);

    };

    class GravitySystem : public DFW::DECS::System::Registrar<GravitySystem>
    {
    public:
        GravitySystem() = default;
        virtual ~GravitySystem() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

        void OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event);
    
    private:
        DFW::SharedPtr<DFW::GameClock> _game_clock;
    };
        
    class LockTetrominoPlacementSystem : public DFW::DECS::System::Registrar<LockTetrominoPlacementSystem>
    {
    public:
        LockTetrominoPlacementSystem() = default;
        virtual ~LockTetrominoPlacementSystem() = default;

    private:
        virtual void Init(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Terminate(DFW::DECS::EntityRegistry& a_registry) override;
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

        void OnTetrominoSpawnedEvent(TetrominoSpawnedEvent& a_event);
    
    private:
        DFW::SharedPtr<DFW::GameClock> _game_clock;
    };

    class SpawnSystem : public DFW::DECS::System::Registrar<SpawnSystem>
    {
    public:
        SpawnSystem() = default;
        virtual ~SpawnSystem() = default;

    private:
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

    };


} // End of namespace ~ Tetriys.
