#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unistd.h>
#include "Map.h"

const   int     MICRO     = 1000000;
const   double  FPS       = (1/30) * (MICRO);
const   int     WIDTH     = 640;
const   int     HEIGHT    = 480;
const   char    TITLE[5]  = "Gonp";

using namespace sf;

int main(int argc, char const *argv[])
{
    //Create game window
    std::shared_ptr<RenderWindow> window = std::make_shared<RenderWindow>(sf::VideoMode(WIDTH,HEIGHT), TITLE);

    //Create and set camera
    sf::View camera(sf::FloatRect(WIDTH, WIDTH, HEIGHT, HEIGHT));
    camera.setCenter(0.f, 0.f);
    window->setView(camera);

    //Create game map
    std::unique_ptr<Map> gameMap = std::make_unique<Map>();
    gameMap->addStage();
    gameMap->addStage();
    gameMap->addStage();
    gameMap->addStage();

    //GAME START
    while (window->isOpen())
    {
        //will be replaced with animations later
        bool should_sleep = false;

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

        //move
        if (Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            gameMap->traverseRight();
            should_sleep = true;
        }

        //set position of camera
        camera.setCenter(gameMap->getCurrentStagePos());
        window->setView(camera);

        //draw the current screen
        gameMap->drawMap(window);

        //display the screen
        window->display();

        if(should_sleep){
            usleep(0.25 * MICRO);
        }

        //set the FPS
        usleep(FPS);
    }

    return 0;
}
