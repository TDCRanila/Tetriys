#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/System.h>

namespace Tetriys
{
    struct PlayField;

    class PlayFieldControlSystem : public DFW::DECS::System::Registrar<PlayFieldControlSystem>
    {
    public:
        void InsertBlockInGrid(PlayField& a_playfield, DFW::Entity const& a_block, BlockCoordinate const& a_coordinate, bool const a_override_block);

        void DestroyBlockInGrid(PlayField& a_playfield, DFW::Entity& a_block);
        void DestroyBlockInGrid(PlayField& a_playfield, BlockCoordinate const& a_coordinate);

        void RemoveBlockInGrid(PlayField& a_playfield, DFW::Entity& a_block);
        void RemoveBlockInGrid(PlayField& a_playfield, BlockCoordinate const& a_coordinate);

        void MoveBlockInGrid(PlayField& a_playfield, DFW::Entity& a_block, BlockCoordinate const& a_new_coordinate, bool const a_override_block);
        void MoveBlockInGrid(PlayField& a_playfield, BlockCoordinate const& a_current_coordinate, BlockCoordinate const& a_new_coordinate, bool const a_override_block);

    private:
        virtual void Update(DFW::DECS::EntityRegistry& a_registry) override;

    };

} // End of namespace ~ Tetriys.
