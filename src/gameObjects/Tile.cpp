#include "Tile.hpp"

Tile::Tile(const std::string &filePath, const Color &pColor) : GameObject(filePath), color(pColor), originalSideSize(0) {
    setDimensions({size - 1, size - 1});
}

Color Tile::getColor() const {
    return color;
}
int Tile::getSize() {
    return size;
}
