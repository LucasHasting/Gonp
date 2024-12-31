#include <SFML/Graphics.hpp>
#include <unistd.h>

#include "../class_headers/Map.h"
#include "../class_headers/Game.h"
#include "../class_headers/GameConstants.h"

using namespace sf;

Game::Game(){
    //set the first game state
    gameState = 1;

    //allocate necessary variables
    window = std::make_shared<RenderWindow>(sf::VideoMode(WIDTH,HEIGHT), TITLE);
    camera = std::make_shared<View>(FloatRect(WIDTH, WIDTH, HEIGHT, HEIGHT));

    //set the camera to the window
    camera->setCenter(0.f, 0.f);
    window->setView(*camera);

    //allocate/create the maps in the game
    initializeMaps();

    //set the current map to the first area
    currentMap = area1;
}

int Game::getGameState(){
    return gameState;
}

void Game::setGameState(int gameState){
    this->gameState = gameState;
}

void Game::driverShell(){
    while (window->isOpen())
    {
        //check to close the game
        sf::Event event;
        while (window->pollEvent(event))
        {
            if(event.type == sf::Event::Closed){
                window->close();
            }
        }

        //clear the screen
        window->clear();

        //use the current state's driver function
        gameDriver();

        //set the FPS
        usleep(FPS);
    }
}

void Game::gameDriver(){
    //use the current state's driver function
    switch(gameState){
        case 1:
            
        case 2:
            currentMap->driver(window, camera);
    }
}

void Game::initializeMaps(){
    //create the area 1 map
    area1 = std::make_shared<Map>();
    area1->addStage(1, 'N');
    area1->addStage(1, 'W');
    area1->addStage(1, 'E');
    area1->addStage(2, 'E'); 
}