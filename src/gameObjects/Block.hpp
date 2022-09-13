#ifndef PUYOPUYO_BLOCK_HPP
#define PUYOPUYO_BLOCK_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "GameObject.hpp"
#include "Tile.hpp"
//#include "utils/Color.hpp"


class Block : public Tile {
public:
    Block(const std::string &filePath, Color pColor);
};


#endif//PUYOPUYO_BLOCK_HPP
