#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

using namespace sf;

class Stage{
    public:
        std::shared_ptr<Stage> up = nullptr;
        std::shared_ptr<Stage> down = nullptr;
        std::shared_ptr<Stage> left = nullptr;
        std::shared_ptr<Stage> right = nullptr;
        bool is_drawn = false;
        std::unique_ptr<Sprite> stageSprite = nullptr;
        int stageNumber = -1;
};

class Map{
    private:
        std::string spriteLocation = "sprites/map/";
        std::string stageSpriteName = spriteLocation + "stage.png";
        std::shared_ptr<Stage> currentStage = nullptr;
        std::shared_ptr<Stage> stageList = nullptr;
        std::unique_ptr<Sprite> getSprite(std::string);
        int max_stage_number = 0;
    public:
        Map();
        void drawMap(std::shared_ptr<sf::RenderWindow>, std::shared_ptr<Stage>, char);
        std::shared_ptr<Stage> searchStage(std::shared_ptr<Stage>, int, char);
        void addStage(int, char);
        Vector2f getCurrentStagePos();
        std::shared_ptr<Stage> getCurrentStage();
        void traverseRight();
        void traverseUp();
        void traverseDown();
        void traverseLeft();
};

#endif
