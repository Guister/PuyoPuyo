#ifndef PUYOPUYO_TILE_HPP
#define PUYOPUYO_TILE_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "GameObject.hpp"
#include "utils/Color.hpp"

/*
 * Represent square game objects.
 * Each tile has 40 pixels for a side.
 * Square game objects have 39 pixels.
 * This is so to make objects not touch and overlap each other.
 */
class Tile : public GameObject {
protected:
    Color color;
    int originalSideSize;
    inline static const int size = 40;

public:
    Tile() = default;
    Tile(const std::string &filePath, const Color &pColor);

    [[nodiscard]] Color getColor() const;
    static int getSize();
};


#endif//PUYOPUYO_TILE_HPP
