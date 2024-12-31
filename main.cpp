#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unistd.h>
#include "class_headers/Map.h"
#include "class_headers/GameConstants.h"

using namespace sf;

int main(int argc, char const *argv[])
{
    //Create game window
    std::shared_ptr<RenderWindow> window = std::make_shared<RenderWindow>(sf::VideoMode(WIDTH,HEIGHT), TITLE);

    //Create and set camera
    std::shared_ptr<View> camera = std::make_shared<View>(FloatRect(WIDTH, WIDTH, HEIGHT, HEIGHT));
    camera->setCenter(0.f, 0.f);
    window->setView(*camera);

    //Create game map
    std::unique_ptr<Map> gameMap = std::make_unique<Map>();
    gameMap->addStage(1, 'N');
    gameMap->addStage(1, 'W');
    gameMap->addStage(1, 'E');
    gameMap->addStage(2, 'E'); 

    //GAME START
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

        gameMap->driver(window, camera);

        //set the FPS
        usleep(FPS);
    }

    return 0;
}
