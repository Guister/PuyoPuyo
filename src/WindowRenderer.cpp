#include "WindowRenderer.hpp"
#include <iostream>

WindowRenderer::WindowRenderer() : screenSize({1080, 1000}) {
    window = SDL_CreateWindow(
            "My window",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screenSize.first, screenSize.second, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Could not create window: " << SDL_GetError() << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
    }
}

SDL_Rect WindowRenderer::buildSDL_Rect(int x, int y, SDL_Texture *texture) {
    SDL_Rect rectangle;
    rectangle.x = x;
    rectangle.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &rectangle.w, &rectangle.h);
    return rectangle;
}

SDL_Texture *WindowRenderer::loadTexture(const char *filePath) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, filePath);
    if (texture == nullptr) {
        std::cout << "Could not load texture: " << SDL_GetError() << std::endl;
    }
    return texture;
}

void WindowRenderer::render(SDL_Texture *texture, const SDL_Rect &src, const SDL_Rect &dst) {
    SDL_RenderCopy(renderer, texture, &src, &dst);
}

void WindowRenderer::render(SDL_Texture *texture, const SDL_Rect &dst) {
    SDL_RenderCopy(renderer, texture, nullptr, &dst);
}

void WindowRenderer::render(SDL_Texture *texture, const SDL_Rect &src, const SDL_Rect &dst, double angle,
                            const SDL_Point *rotationPoint, const SDL_RendererFlip &flags) {
    SDL_RenderCopyEx(renderer, texture, &src, &dst, angle, rotationPoint, flags);
}

void WindowRenderer::clear() {
    SDL_RenderClear(renderer);
}

void WindowRenderer::display() {
    SDL_RenderPresent(renderer);
}

void WindowRenderer::shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

WindowRenderer &WindowRenderer::getWindowRenderer() {
    static WindowRenderer onlyInstance;
    return onlyInstance;
}

std::pair<int, int> WindowRenderer::getScreenSize() const {
    return screenSize;
}
std::pair<int, int> WindowRenderer::getScreenCenter() const {
    return std::make_pair(screenSize.first / 2, screenSize.second / 2);
}
