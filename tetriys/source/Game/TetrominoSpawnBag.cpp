#include <Game/TetrominoSpawnBag.h>

#include <DFW/CoreSystems/Logging/Logger.h>

namespace Tetriys
{
    void TetrominoSpawnBag::PrintTetrominoList() const
    {
        std::string random_list_string = "";

        for (TetrominoType const& tetromino_type : tetromino_bag)
        {
            random_list_string += std::to_string(static_cast<size_t>(tetromino_type));
        }

        DFW_INFOLOG("TetrominoBagList: {}", random_list_string);
    }

} // End of namespace ~ Tetriys.
