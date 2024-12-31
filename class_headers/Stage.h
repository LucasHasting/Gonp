#ifndef STAGE_H
#define STAGE_H

#include <SFML/Graphics/Sprite.hpp>
#include "NewSprite.h"

using namespace sf;

struct Stage{
        std::shared_ptr<Stage> up = nullptr;
        std::shared_ptr<Stage> down = nullptr;
        std::shared_ptr<Stage> left = nullptr;
        std::shared_ptr<Stage> right = nullptr;
        bool isDrawn = false;
        std::unique_ptr<NewSprite> stageSprite = nullptr;
        std::unique_ptr<NewSprite> stageTrail = nullptr;
        int stageNumber = -1;
};

#endif