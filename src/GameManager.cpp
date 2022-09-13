#include <algorithm>
#include <random>
#include <set>

#include "GameManager.hpp"
#include "gameObjects/Ball.hpp"
#include "utils/Color.hpp"

GameManager::GameManager() : windowRenderer(WindowRenderer::getWindowRenderer()), audioMixer(AudioMixer::getAudioMixer()),
                             background(filesystem::GetSourceFolder().string() + "/assets/images/tilesetOpenGameBackground.png") {
    tickDuration = 1000.0f / static_cast<float>(SDL_GetPerformanceFrequency());
    frameTicks = static_cast<float>(SDL_GetPerformanceFrequency()) * 1.0f / 60.0f;
    timer = SDL_GetPerformanceCounter();
    playerHasControl = true;
    play = true;
    pause = false;
    doubleCheck = false;
}

void GameManager::init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << std::endl;
    }
    if (!IMG_Init(IMG_INIT_PNG)) {
        std::cout << "Unable to initialize IMG: " << SDL_GetError() << std::endl;
    }
    if (!Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3 | MIX_INIT_FLAC)) {
        std::cout << "Unable to initialize Mixer: " << SDL_GetError() << std::endl;
    }
}

void GameManager::buildGrid() {
    grid = Grid(windowRenderer.getScreenCenter(), 8, 16);
}

void GameManager::spawnBallPair() {
    std::pair<int, int> screenCenter = windowRenderer.getScreenCenter();
    std::pair<int, int> spawnPositionBall1 =
            std::make_pair(screenCenter.first - Tile::getSize(), grid.getInitialPoint().second - Tile::getSize());
    std::pair<int, int> spawnPositionBall2 =
            std::make_pair(spawnPositionBall1.first + Tile::getSize(), spawnPositionBall1.second);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<uint32_t> chances = {1, 1, 1, 1};

    Ball ball1(filesystem::GetSourceFolder().string() + "/assets/images/sprites.png", static_cast<Color>(chances(gen)));
    ball1.setPosition(spawnPositionBall1);
    balls.push_back(ball1);
    Ball ball2(filesystem::GetSourceFolder().string() + "/assets/images/sprites.png", static_cast<Color>(chances(gen)), true);
    ball2.setPosition(spawnPositionBall2);
    balls.push_back(ball2);
}

void GameManager::instantiateGameObjects() {
    background.setDimensions(windowRenderer.getScreenSize());
    buildGrid();
    spawnBallPair();
}

void GameManager::moveBallPair(Direction direction) {
    Ball &ball1 = balls.rbegin()[1];
    Ball &ball2 = balls.back();

    int leftLimit = grid.getInitialPoint().first + Tile::getSize();
    int rightLimit = grid.getEndPoint().first - Tile::getSize();
    bool insideLeftBounds = ball1.getDstRectanglePtr()->x > leftLimit && ball2.getDstRectanglePtr()->x > leftLimit;
    bool insideRightBounds = ball1.getDstRectanglePtr()->x < rightLimit && ball2.getDstRectanglePtr()->x < rightLimit;

    bool cantGoLeft = false;
    bool cantGoRight = false;
    auto vec = getBallsExceptLast(2);
    for (auto &ball: vec) {
        int y1Diff = std::abs(ball.getDstRectangle().y - ball1.getDstRectangle().y);
        int y2Diff = std::abs(ball.getDstRectangle().y - ball2.getDstRectangle().y);
        if (!cantGoLeft) {
            int x1Diff = ball1.getDstRectangle().x - ball.getDstRectangle().x;
            int x2Diff = ball2.getDstRectangle().x - ball.getDstRectangle().x;
            cantGoLeft = (x1Diff == Tile::getSize() || x2Diff == Tile::getSize()) && (y1Diff <= Tile::getSize() || y2Diff <= Tile::getSize());
        }
        if (!cantGoRight) {
            int x1Diff = ball.getDstRectangle().x - ball1.getDstRectangle().x;
            int x2Diff = ball.getDstRectangle().x - ball2.getDstRectangle().x;
            cantGoRight = (x1Diff == Tile::getSize() || x2Diff == Tile::getSize()) && (y1Diff <= Tile::getSize() || y2Diff <= Tile::getSize());
        }
    }

    bool canMoveLeft = insideLeftBounds && !cantGoLeft;
    bool canMoveRight = insideRightBounds && !cantGoRight;

    if (direction == Direction::left && canMoveLeft) {
        ball1.move(Direction::left);
        ball2.move(Direction::left);
        audioMixer.playSound("bounce");
    } else if (direction == Direction::right && canMoveRight) {
        ball1.move(Direction::right);
        ball2.move(Direction::right);
        audioMixer.playSound("bounce");
    }
}

void GameManager::rotateBallPair() {
    Ball &ball1 = balls.rbegin()[1];
    Ball &ball2 = balls.back();
    auto gridInitialPoint = grid.getInitialPoint();
    auto gridEndPoint = grid.getEndPoint();

    bool rotate0Border = ball2.getDstRectanglePtr()->y < gridEndPoint.second - Tile::getSize() * 2;
    bool rotate90Border = ball2.getDstRectanglePtr()->x > gridInitialPoint.first + Tile::getSize();
    bool rotate270Border = ball2.getDstRectanglePtr()->x < gridEndPoint.first - Tile::getSize();

    uint32_t angleRemainder = ball2.getAngleRemainder();
    bool canRotate = true;
    auto ballVec = getBallsExceptSecondToLast();
    for (auto &ball: ballVec) {
        int xDiff = ball.getDstRectanglePtr()->x - ball1.getDstRectanglePtr()->x;
        int yDiff = ball.getDstRectanglePtr()->y - ball1.getDstRectanglePtr()->y;
        bool rotatingIntoAnotherBall;
        if (canRotate) {
            switch (angleRemainder) {
                case 0:
                    rotatingIntoAnotherBall = xDiff == 0 && yDiff < Tile::getSize() * 2;
                    canRotate = rotate0Border && !rotatingIntoAnotherBall;
                    break;
                case 90:
                    rotatingIntoAnotherBall = xDiff == -Tile::getSize() && yDiff < Tile::getSize();
                    canRotate = rotate90Border && !rotatingIntoAnotherBall;
                    break;
                case 180:
                    canRotate = true;
                    break;
                case 270:
                    rotatingIntoAnotherBall = xDiff == Tile::getSize() && yDiff < Tile::getSize();
                    canRotate = rotate270Border && !rotatingIntoAnotherBall;
                    break;
                default:
                    canRotate = false;
                    break;
            }
        }
    }
    if (canRotate) {
        ball2.rotate();
        audioMixer.playSound("bounce");
    }
}

void GameManager::accelerateBallPair() {
    Ball &ball1 = balls.rbegin()[1];
    Ball &ball2 = balls.back();
    uint32_t velocity = 5;
    int downLimit = grid.getEndPoint().second - Tile::getSize();
    bool canAccelerate = ball1.getDstRectanglePtr()->y < downLimit && ball2.getDstRectanglePtr()->y < downLimit;

    if (canAccelerate) {
        ball1.setDescendingVelocity(velocity);
        ball2.setDescendingVelocity(velocity);
    } else {
        ball1.setDescendingVelocity(2);
        ball2.setDescendingVelocity(2);
    }
}

void GameManager::resetBallPairVelocity() {
    Ball &ball1 = balls.rbegin()[1];
    Ball &ball2 = balls.back();
    ball1.setDescendingVelocity(2);
    ball2.setDescendingVelocity(2);
}


void GameManager::manageEvents(bool &gameIsRunning) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        SDL_Keycode keyPressed;
        switch (event.type) {
            case SDL_KEYDOWN:
                if (playerHasControl) {
                    keyPressed = event.key.keysym.sym;
                    if (keyPressed == SDLK_LEFT) {
                        moveBallPair(Direction::left);
                    } else if (keyPressed == SDLK_RIGHT) {
                        moveBallPair(Direction::right);
                    } else if (keyPressed == SDLK_UP) {
                        rotateBallPair();
                    } else if (keyPressed == SDLK_DOWN) {
                        accelerateBallPair();
                    }
                }
                break;
            case SDL_KEYUP:
                if (playerHasControl) {
                    keyPressed = event.key.keysym.sym;
                    if (keyPressed == SDLK_DOWN) {
                        resetBallPairVelocity();
                    }
                }
                break;
            case SDL_QUIT:
                gameIsRunning = false;
                break;
            default:
                break;
        }
    }
}

void GameManager::controlFall(int downLimit) {
    auto &ball1 = balls.rbegin()[1];
    auto &ball2 = balls.back();
    bool falling1 = ball1.getDstRectanglePtr()->y < downLimit && !ball1.checkColision(balls, ball1);
    bool falling2 = ball2.getDstRectanglePtr()->y < downLimit && !ball2.checkColision(balls, ball2);
    if (falling1) {
        ball1.fall();
    }
    if (falling2) {
        ball2.fall();
    }
    if (!falling1) {
        ball1.applySafetyNet(downLimit);
        ball1.setDescendingVelocity(0);
        ball2.setDescendingVelocity(5);
        ball1.lock();
        playerHasControl = false;
    }
    if (!falling2) {
        ball2.applySafetyNet(downLimit);
        ball2.setDescendingVelocity(0);
        ball1.setDescendingVelocity(5);
        ball2.lock();
        playerHasControl = false;
    }

    if (ball1.isLocked() && ball2.isLocked()) {
        play = false;
        ball1.setDescendingVelocity(0);
        ball2.setDescendingVelocity(0);
    }
}

void GameManager::destroyBalls() {

    std::vector<std::set<Ball>> combosSet(balls.size());

    // Create sets with balls with the same color adjacent to each other
    for (uint32_t i = 0; i < balls.size(); i++) {
        auto ballI = balls.at(i);
        auto it = std::find_if(combosSet.begin(), combosSet.end(), [&ballI](const std::set<Ball> &set) {
            return set.find(ballI) != set.end();
        });
        if (it != combosSet.end()) {
            continue;
        } else {
            combosSet.at(i).insert(ballI);
            checkRecursively(i, i, combosSet);
        }
    }

    // Remove the set from the balls vector
    for (auto &set: combosSet) {
        if (set.size() >= 4) {
            for (const auto &ball: set) {
                auto it = std::find(balls.begin(), balls.end(), ball);
                if (it != balls.end()) {
                    balls.at(std::distance(balls.begin(), it)).shutdown();
                    balls.erase(it);
                    audioMixer.playSound("pop");
                }
            }
        }
    }
}

void GameManager::checkRecursively(uint32_t source, uint32_t next, std::vector<std::set<Ball>> &combosSet) {

    //If there is a match let's check the neighbours of the matched ball
    for (uint32_t j = 0; j < balls.size(); j++) {
        auto ballI = balls.at(next);
        auto ballJ = balls.at(j);
        if (ballI.matches(ballJ)) {
            auto it = std::find_if(combosSet.begin(), combosSet.end(), [&ballJ](const std::set<Ball> &set) {
                return set.find(ballJ) != set.end();
            });
            if (it != combosSet.end()) {
                continue;
            } else {
                combosSet.at(source).insert(ballJ);
                checkRecursively(source, j, combosSet);
            }
        }
    }
}

void GameManager::makeBallsFallAfterDestruction(int downLimit) {

    //Let's sort to make the balls closer to the ground go down first.
    std::sort(balls.begin(), balls.end(), [](const Ball &b1, const Ball &b2) {
        return b1.getDstRectangle().y > b2.getDstRectangle().y;
    });
    for (auto &ball: balls) {
        if (!(ball.checkBallBelow(balls)) && ball.getDstRectanglePtr()->y < downLimit) {
            ball.setDescendingVelocity(5);
            ball.fall();
        } else {
            ball.applySafetyNet(downLimit);
            ball.setDescendingVelocity(0);
        }
    }
}

void GameManager::resetGameLoop() {
    bool ballsAreFalling = std::any_of(balls.begin(), balls.end(), [](const Ball &b) {
        return b.getDescendingVelocity() != 0;
    });

    if (doubleCheck && !ballsAreFalling) {
        bool lost = std::any_of(balls.begin(), balls.end(), [this](const Ball &b) {
            return b.getDstRectangle().y < grid.getInitialPoint().second + Tile::getSize() * 2;
        });
        if (!lost) {//Continue the game normally
            spawnBallPair();
            playerHasControl = true;
            play = true;
        } else {//Reset the game
            for (auto &ball: balls) {
                ball.shutdown();
            }
            balls.clear();
        }
    }

    // We need to make sure that !ballsAreFalling for 2 game loops in a row.
    // Otherwise, the game will not check for destruction again when !ballsAreFalling for the first loop.
    if (!ballsAreFalling) {
        doubleCheck = true;
    } else {
        doubleCheck = false;
    }
}

void GameManager::update() {
    if (!pause) {
        int downLimit = grid.getEndPoint().second - Tile::getSize();
        if (play) {
            controlFall(downLimit);
        } else if (!play) {
            destroyBalls();
            makeBallsFallAfterDestruction(downLimit);
            resetGameLoop();
        }
    } else if (pause) {
    }
}

void GameManager::clear() {
    windowRenderer.clear();
}

void GameManager::drawGameObjects() {
    background.draw();
    for (auto &ball: balls) {
        ball.draw();
    }
    for (auto &block: grid.getGrid()) {
        block.draw();
    }
}

void GameManager::display() {
    windowRenderer.display();
}

void GameManager::controlFPS() {
    auto now = SDL_GetPerformanceCounter();
    float delay = frameTicks - static_cast<float>(now - timer);
    if (delay > 0) {
        SDL_Delay(static_cast<uint32_t>(delay * tickDuration));
    }
    timer = SDL_GetPerformanceCounter();
}

void GameManager::shutdown() {
    background.shutdown();
    for (auto &block: grid.getGrid()) {
        block.shutdown();
    }
    for (auto &ball: balls) {
        ball.shutdown();
    }
    windowRenderer.shutdown();
    audioMixer.shutdown();
    SDL_Quit();
}

GameManager &GameManager::getGameManager() {
    static GameManager onlyInstance;
    return onlyInstance;
}

std::vector<Ball> GameManager::getBallsExceptLast(int number) {
    auto temp = balls;
    for (int i = 0; i < number; i++) {
        temp.erase(temp.end());
    }
    return temp;
}
std::vector<Ball> GameManager::getBallsExceptSecondToLast() {
    auto temp = balls;
    temp.erase(temp.end() - 2);
    return temp;
}