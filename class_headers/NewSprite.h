#ifndef NEW_SPRITE_H
#define NEW_SPRITE_H

#include <SFML/Graphics/Sprite.hpp>

using namespace sf;

struct NewSprite{
    std::unique_ptr<Sprite> sprite;
    std::unique_ptr<Texture> texture;
};

#endif