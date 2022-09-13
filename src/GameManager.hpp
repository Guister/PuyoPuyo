#ifndef PUYOPUYO_GAMEMANAGER_HPP
#define PUYOPUYO_GAMEMANAGER_HPP

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "SDL2/SDL_mixer.h"
#include <SDL2/SDL_image.h>
#include <memory>
#include <set>
#include <vector>

#include "AudioMixer.hpp"
#include "WindowRenderer.hpp"
#include "gameObjects/Ball.hpp"
#include "gameObjects/Block.hpp"
#include "gameObjects/GameObject.hpp"
#include "gameObjects/Grid.hpp"
#include "gameObjects/Tile.hpp"
#include "utils/FileSystem.hpp"

/*
 * Responsable to deal with all game events.
 * Singleton.
 */

class GameManager {
private:
    WindowRenderer &windowRenderer;
    AudioMixer &audioMixer;

    GameObject background;
    Grid grid;

    std::vector<Ball> balls;

    float tickDuration;
    float frameTicks;
    uint64_t timer;
    bool playerHasControl;
    bool play;
    bool pause;
    bool doubleCheck;

    GameManager();
    ~GameManager() = default;

    void buildGrid();
    void spawnBallPair();
    void moveBallPair(Direction direction);
    void rotateBallPair();
    void accelerateBallPair();
    void controlFall(int downLimit);
    void destroyBalls();
    void checkRecursively(uint32_t source, uint32_t next, std::vector<std::set<Ball>> &combosSet);
    void makeBallsFallAfterDestruction(int downLimit);
    void resetGameLoop();
    void resetBallPairVelocity();

public:
    GameManager(const GameManager &) = delete;
    GameManager &operator=(const GameManager &) = delete;
    GameManager(GameManager &&) = delete;
    GameManager &operator=(GameManager &&) = delete;

    static void init(); //Must be the first function to be called in main.

    void instantiateGameObjects();
    void manageEvents(bool &gameIsRunning);
    void update();
    void clear();
    void drawGameObjects();
    void display();
    void controlFPS();
    void shutdown();

    static GameManager &getGameManager();

    std::vector<Ball> getBallsExceptLast(int number = 1);
    std::vector<Ball> getBallsExceptSecondToLast();
};


#endif//PUYOPUYO_GAMEMANAGER_HPP
