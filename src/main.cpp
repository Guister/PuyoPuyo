#include "GameManager.hpp"


int main() {

    GameManager::init();

    GameManager &game = GameManager::getGameManager();

    bool gameIsRunning = true;

    game.instantiateGameObjects();

    while (gameIsRunning) {
        game.manageEvents(gameIsRunning);
        game.update();
        game.clear();
        game.drawGameObjects();
        game.display();
        game.controlFPS();
    }

    game.shutdown();

    return 0;
}