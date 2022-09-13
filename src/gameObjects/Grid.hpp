#ifndef PUYOPUYO_GRID_HPP
#define PUYOPUYO_GRID_HPP

#include <memory>
#include <vector>

#include "Block.hpp"

/*
 * Represents the play area where our balls will fall.
 * Rectangle composed by random colored Blocks.
 */
class Grid {
private:
    std::vector<Block> grid;
    std::pair<int, int> initialPoint;
    int lenght;
    int height;
    std::pair<int, int> endPoint;

public:
    Grid() = default;
    Grid(const std::pair<int, int> &screenCenter, int pLenght, int pHeight);
    [[nodiscard]] std::vector<Block> &getGrid();

    [[nodiscard]] const std::pair<int, int> &getInitialPoint() const;
    [[nodiscard]] const std::pair<int, int> &getEndPoint() const;
};


#endif//PUYOPUYO_GRID_HPP
