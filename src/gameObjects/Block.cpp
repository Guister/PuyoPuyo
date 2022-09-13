#include <iostream>

#include "Block.hpp"

Block::Block(const std::string &filePath, Color pColor) : Tile(filePath, pColor) {
    originalSideSize = srcRectangle.h / 3;  // Image file uses 3 dimensions of height to represent objects.
    srcRectangle.y = originalSideSize;
    srcRectangle.w = originalSideSize;
    srcRectangle.h = originalSideSize;
    //Let's find the right Block
    switch (color) {
        case Color::red:
            srcRectangle.x = originalSideSize;
            break;
        case Color::lightGreen:
            srcRectangle.x = srcRectangle.w * 3;
            break;
        case Color::grey:
            srcRectangle.x = srcRectangle.w * 5;
            break;
        case Color::yellow:
            srcRectangle.x = srcRectangle.w * 7;
            break;
        case Color::green:
            srcRectangle.x = srcRectangle.w * 9;
            break;
        case Color::blue:
            srcRectangle.x = srcRectangle.w * 11;
            break;
        default:
            std::cout << "Tried to create a block of illegal color" << std::endl;
            break;
    }
}
