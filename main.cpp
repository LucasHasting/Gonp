#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <unistd.h>
#include "class_headers/Map.h"
#include "class_headers/Game.h"
#include "class_headers/GameConstants.h"

using namespace sf;

int main(int argc, char const *argv[])
{
    std::unique_ptr<Game> game = std::make_unique<Game>();
    game->driverShell();
    return 0;
}
