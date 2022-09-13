#include <algorithm>
#include <iostream>

#include "Ball.hpp"

Ball::Ball(const std::string &filePath, const Color &pColor, bool isFlipped) : Tile(filePath, pColor), locked(false), flipped(isFlipped),
                                                                               descendingVelocity(2), angle(0) {
    originalSideSize = srcRectangle.h;  // Image file uses 1 dimension of height to represent objects.
    srcRectangle.w = srcRectangle.h;

    //Let's find the right Ball
    switch (color) {
        case Color::blue:
            srcRectangle.x = originalSideSize * 0;
            break;
        case Color::yellow:
            srcRectangle.x = originalSideSize * 2;
            break;
        case Color::red:
            srcRectangle.x = originalSideSize * 4;
            break;
        case Color::orange:
            srcRectangle.x = originalSideSize * 6;
            break;
        default:
            std::cout << "Tried to create a ball of illegal color" << std::endl;
            break;
    }
}

bool Ball::operator==(const Ball &rhs) const {
    return dstRectangle.x == rhs.getDstRectangle().x &&
           dstRectangle.y == rhs.getDstRectangle().y &&
           color == rhs.color;
}

bool Ball::operator!=(const Ball &rhs) const {
    return !(dstRectangle.x == rhs.getDstRectangle().x &&
             dstRectangle.y == rhs.getDstRectangle().y &&
             color == rhs.color);
}

bool Ball::operator<(const Ball &rhs) const {
    if (dstRectangle.x != rhs.dstRectangle.x)
        return dstRectangle.x < rhs.dstRectangle.x;
    return dstRectangle.y < rhs.dstRectangle.y;
}

bool Ball::checkColision(std::vector<Ball> gameObjects, const Ball &instance) {
    //Let's remove the instance from the equation.
    auto it = std::find(gameObjects.begin(), gameObjects.end(), instance);
    if (it != gameObjects.end()) {
        gameObjects.erase(it);
    }
    bool colliding = std::any_of(gameObjects.begin(), gameObjects.end(), [this](Ball &gameObject) -> bool {
        return SDL_HasIntersection(&dstRectangle, gameObject.getDstRectanglePtr());
    });
    return colliding;
}

bool Ball::checkBallBelow(std::vector<Ball> gameObjects) {
    bool hasBallBelow = std::any_of(gameObjects.begin(), gameObjects.end(), [this](const Ball &ball) {
        return this->getDstRectangle().x == ball.getDstRectangle().x &&
               this->getDstRectangle().y + Tile::getSize() == ball.getDstRectangle().y;
    });
    return hasBallBelow;
}

bool Ball::matches(Ball ball) {
    int xDist = std::abs(dstRectangle.x - ball.getDstRectanglePtr()->x);
    int yDist = std::abs(dstRectangle.y - ball.getDstRectanglePtr()->y);
    //Conditions to match: ball adjacent with same color that is not moving.
    return ((xDist == Tile::getSize() && yDist == 0) ||
            (xDist == 0 && yDist == Tile::getSize())) &&
           color == ball.getColor() &&
           descendingVelocity == 0 &&
           ball.getDescendingVelocity() == 0;
}

void Ball::draw() {
    if (flipped) {
        GameObject::draw(0, nullptr, SDL_FLIP_HORIZONTAL);
    } else {
        GameObject::draw();
    }
}

void Ball::move(Direction direction) {
    if (direction == Direction::left) {
        GameObject::move({-size, 0});
    } else if (direction == Direction::right) {
        GameObject::move({size, 0});
    }
}
void Ball::rotate() {
    angle += 90;
    uint32_t angleRemainder = getAngleRemainder();
    switch (angleRemainder) {
        case 90:
            dstRectangle.x -= Tile::getSize();
            dstRectangle.y += Tile::getSize();
            break;
        case 180:
            dstRectangle.x -= Tile::getSize();
            dstRectangle.y -= Tile::getSize();
            break;
        case 270:
            dstRectangle.x += Tile::getSize();
            dstRectangle.y -= Tile::getSize();
            break;
        case 0:
            dstRectangle.x += Tile::getSize();
            dstRectangle.y += Tile::getSize();
            break;
        default:
            std::cout << "Something wrong with rotation.";
    }
}
void Ball::fall() {
    GameObject::move({0, descendingVelocity});
}
void Ball::applySafetyNet(int refencePoint) {
    //Let's place back the Ball in a legal Tile position in case it was moving too fast
    int offset = refencePoint % size;
    int correctPosition = dstRectangle.y / size;
    correctPosition = correctPosition * size + offset;
    if (dstRectangle.y > correctPosition) {
        dstRectangle.y = correctPosition;
    }
}

bool Ball::isLocked() const {
    return locked;
}
uint32_t Ball::getDescendingVelocity() const {
    return descendingVelocity;
}
uint32_t Ball::getAngle() const {
    return angle;
}
uint32_t Ball::getAngleRemainder() const {
    return angle % 360;
}

void Ball::lock() {
    locked = true;
}
void Ball::unlock() {
    locked = false;
}
void Ball::setDescendingVelocity(uint32_t velocity) {
    descendingVelocity = velocity;
}
