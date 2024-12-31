#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics/Sprite.hpp>
#include <memory>
#include "NewSprite.h"
#include "Stage.h"
#include <string>

using namespace sf;

class Map{
    private:
        std::string spriteLocation = "sprites/map/";
        std::string stageSpriteName = spriteLocation + "stage.png";
        std::string stageTrailName = spriteLocation + "stageTrail.png";
        std::shared_ptr<Stage> currentStage = nullptr;
        std::shared_ptr<Stage> stageList = nullptr;
        std::unique_ptr<NewSprite> getSprite(std::string);
        int max_stage_number = 0;
    public:
        Map();
        void driver(std::shared_ptr<sf::RenderWindow>, std::shared_ptr<View>);
        void drawMap(std::shared_ptr<sf::RenderWindow>, std::shared_ptr<Stage>, char);
        std::shared_ptr<Stage> searchStage(std::shared_ptr<Stage>, int, char);
        void addStage(int, char);
        Vector2f getCurrentStagePos();
        std::shared_ptr<Stage> getCurrentStage();
        void traverseRight();
        void traverseUp();
        void traverseDown();
        void traverseLeft();
        void drawSurrounding();
};

#endif
