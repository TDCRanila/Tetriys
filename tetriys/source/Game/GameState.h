#pragma once

#include <DFW/Modules/ECS/Component.h>

#include <boost/random/mersenne_twister.hpp>

namespace Tetriys
{
    enum class GameState
    {
          NONE
        , START_GAME
        , PAUSE_GAME
        , END_GAME
        , PLAY_GAME

    };

    enum class PlayState
    {
          NONE
        , STARTING
        , SPAWNING
        , PLACING
        , LOCKING
        , CHECKING_LINE_CLEARS
        , HOLDING
    };
    
    struct GameStateComponent : public DFW::DECS::Component::StrictRegistrar<GameStateComponent>
    {
        virtual ~GameStateComponent() = default;

        GameState game_state;
        PlayState play_state;
    };

    struct GameRNG : public DFW::DECS::Component::StrictRegistrar<GameRNG>
    {
        virtual ~GameRNG() = default;

        boost::random::mt19937 rng_engine;
    };

    using GameNameID = std::string;

    struct GameNameIDComponent : public DFW::DECS::Component::StrictRegistrar<GameNameIDComponent>
    {
        virtual ~GameNameIDComponent() = default;

        GameNameID game_id;
    };

} // End of namespace ~ Tetriys.
