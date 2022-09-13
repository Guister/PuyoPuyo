#include "Grid.hpp"
#include "utils/FileSystem.hpp"
#include <random>
#include <utility>

Grid::Grid(const std::pair<int, int> &screenCenter, int pLenght, int pHeight) : lenght(pLenght), height(pHeight) {
    initialPoint = std::make_pair(screenCenter.first - Tile::getSize() * lenght / 2 - Tile::getSize(),
                                  screenCenter.second - Tile::getSize() * height / 2 - Tile::getSize());
    endPoint = std::make_pair(screenCenter.first + Tile::getSize() * lenght / 2,
                              screenCenter.second + Tile::getSize() * height / 2);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<uint32_t> chances = {1, 1, 1, 1, 1, 1};

    for (int i = 0; i < lenght + 2; i++) {
        int xInitialPosition = initialPoint.first + Tile::getSize() * i;
        auto chosenColor = static_cast<Color>(chances(gen) + 1);//There's no orange block
        Block upBlock(filesystem::GetSourceFolder().string() + "/assets/images/blocks.png", chosenColor);
        upBlock.setPosition({xInitialPosition, initialPoint.second});
        grid.push_back(upBlock);
        Block downBlock(filesystem::GetSourceFolder().string() + "/assets/images/blocks.png", chosenColor);
        downBlock.setPosition({xInitialPosition, endPoint.second});
        grid.push_back(downBlock);
    }

    for (int i = 1; i <= height; i++) {
        int yInitialPosition = initialPoint.second + Tile::getSize() * i;
        auto chosenColor = static_cast<Color>(chances(gen) + 1);//There's no orange block
        Block leftBlock(filesystem::GetSourceFolder().string() +  "/assets/images/blocks.png", chosenColor);
        leftBlock.setPosition({initialPoint.first, yInitialPosition});
        grid.push_back(leftBlock);
        Block rightBlock(filesystem::GetSourceFolder().string() +  "/assets/images/blocks.png", chosenColor);
        rightBlock.setPosition({endPoint.first, yInitialPosition});
        grid.push_back(rightBlock);
    }
}

std::vector<Block> &Grid::getGrid() {
    return grid;
}
const std::pair<int, int> &Grid::getInitialPoint() const {
    return initialPoint;
}
const std::pair<int, int> &Grid::getEndPoint() const {
    return endPoint;
}
