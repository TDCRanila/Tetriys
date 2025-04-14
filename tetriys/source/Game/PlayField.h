#pragma once

#include <Game/BlockCoordinate.h>

#include <DFW/Modules/ECS/Entity.h>
#include <DFW/Modules/ECS/Component.h>

#include <DFW/Utility/Grid/StaticGrid2D.h>

#include <glm/glm.hpp>

namespace Tetriys
{
    constexpr size_t TETRIYS_GRID_WIDTH = 10;
    constexpr size_t TETRIYS_GRID_HEIGHT = 40;
    constexpr float32 TETRIYS_BLOCK_SPACING = 2;

    struct PlayField;

    struct PlayFieldRef final : public DFW::DECS::Component::StrictRegistrar<PlayFieldRef>
    {
        PlayFieldRef() : ptr(nullptr) {}
        PlayFieldRef(PlayField& a_playfield_ref) : ptr(&a_playfield_ref) { }

        PlayField& Get() { return *ptr; }
        PlayField const& Get() const { return const_cast<PlayField&>(const_cast<PlayFieldRef*>(this)->Get()); }

    private:
        PlayField* ptr;
    };

    struct PlayFieldDataEntry
    {
        void SyncBlockEntity();
        void ClearBlockEntity();
        void DestroyBlockEntity();

        glm::vec3 block_world_position;
        BlockCoordinate grid_coordinate { 0, 0 };
        DFW::Entity block;
    };

    struct PlayField 
        : public DFW::DUtility::StaticGrid2D<PlayFieldDataEntry, TETRIYS_GRID_WIDTH, TETRIYS_GRID_HEIGHT>
        , public DFW::DECS::Component::StrictRegistrar<PlayField>
    {
        void Setup();
        void Clear();

        PlayFieldDataEntry const& GetDataEntry(BlockCoordinate const& a_coordinate) const { return Get(a_coordinate.x, a_coordinate.y); }
        PlayFieldDataEntry& GetDataEntry(BlockCoordinate const& a_coordinate) { return Get(a_coordinate.x, a_coordinate.y); }
        bool IsValidGridCoordinate(BlockCoordinate const& a_coordinate) const { return IsValidCoordinate(a_coordinate.x, a_coordinate.y); }

    };

} // End of namespace ~ Tetriys.
