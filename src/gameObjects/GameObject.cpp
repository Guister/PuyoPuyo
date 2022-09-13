#include "GameObject.hpp"

GameObject::GameObject(const std::string &filePath) {
    texture = WindowRenderer::loadTexture(filePath.c_str());
    srcRectangle = WindowRenderer::buildSDL_Rect(0, 0, texture);
    dstRectangle = WindowRenderer::buildSDL_Rect(0, 0, texture);
}

void GameObject::draw() {
    if (texture != nullptr) {
        WindowRenderer::render(texture, srcRectangle, dstRectangle);
    }
}

void GameObject::draw(double angle, const SDL_Point *rotationPoint, const SDL_RendererFlip &flipFlag) {
    if (texture != nullptr) {
        WindowRenderer::render(texture, srcRectangle, dstRectangle, angle, rotationPoint, flipFlag);
    }
}

void GameObject::move(const std::pair<int, int> &coordinates) {
    dstRectangle.x += coordinates.first;
    dstRectangle.y += coordinates.second;
}

void GameObject::shutdown() {
    if (texture != nullptr) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

void GameObject::setPosition(const std::pair<int, int> &coordinates) {
    dstRectangle.x = coordinates.first;
    dstRectangle.y = coordinates.second;
}

void GameObject::setDimensions(const std::pair<int, int> &dimensions) {
    dstRectangle.w = dimensions.first;
    dstRectangle.h = dimensions.second;
}

SDL_Rect *GameObject::getDstRectanglePtr() {
    return &dstRectangle;
}
SDL_Rect GameObject::getDstRectangle() const {
    return dstRectangle;
}
