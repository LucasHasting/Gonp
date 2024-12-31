#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Map.h"

using namespace sf;

class Game {
    private:
        int gameState = 1;
        std::shared_ptr<Map> currentMap = nullptr;
        std::shared_ptr<Map> area1 = nullptr;
        std::shared_ptr<View> camera = nullptr;
        std::shared_ptr<RenderWindow> window = nullptr;
    public:
        Game();
        int getGameState();
        void setGameState(int);
        void gameDriver();
        void initializeMaps();
        void driverShell();
};

#endif