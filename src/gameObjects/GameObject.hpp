#ifndef PUYOPUYO_GAMEOBJECT_HPP
#define PUYOPUYO_GAMEOBJECT_HPP

#include "WindowRenderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <utility>

class GameObject {
protected:
    SDL_Texture *texture;
    SDL_Rect srcRectangle;
    SDL_Rect dstRectangle;

public:
    GameObject() = default;
    explicit GameObject(const std::string &filePath);

    virtual void draw();
    void draw(double angle, const SDL_Point *rotationPoint = nullptr, const SDL_RendererFlip &flipFlag = SDL_RendererFlip::SDL_FLIP_NONE);
    void move(const std::pair<int, int> &coordinates);
    void shutdown();

    void setPosition(const std::pair<int, int> &coordinates);
    void setDimensions(const std::pair<int, int> &dimensions);

    [[nodiscard]] SDL_Rect *getDstRectanglePtr();
    [[nodiscard]] SDL_Rect getDstRectangle() const;

    virtual ~GameObject() = default;
};


#endif//PUYOPUYO_GAMEOBJECT_HPP
