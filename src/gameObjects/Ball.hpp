#ifndef PUYOPUYO_BALL_HPP
#define PUYOPUYO_BALL_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

#include "GameObject.hpp"
#include "Tile.hpp"
#include "WindowRenderer.hpp"
#include "utils/Color.hpp"
#include "utils/Direction.hpp"

class Ball : public Tile {
private:
    bool locked;
    bool flipped;
    uint32_t descendingVelocity;
    uint32_t angle;

public:
    Ball() = default;
    Ball(const std::string &filePath, const Color &pColor, bool isFlipped = false);
    bool operator==(const Ball &rhs) const;
    bool operator!=(const Ball &rhs) const;
    bool operator<(const Ball &rhs) const;

    bool checkColision(std::vector<Ball> gameObjects, const Ball &instance);
    bool checkBallBelow(std::vector<Ball> gameObjects);
    bool matches(Ball ball);
    void draw() override;
    void move(Direction direction);
    void rotate();
    void fall();

    void applySafetyNet(int refencePoint);

    [[nodiscard]] bool isLocked() const;
    [[nodiscard]] uint32_t getDescendingVelocity() const;
    [[nodiscard]] uint32_t getAngle() const;
    [[nodiscard]] uint32_t getAngleRemainder() const;

    void lock();
    void unlock();
    void setDescendingVelocity(uint32_t velocity);
};


#endif//PUYOPUYO_BALL_HPP
