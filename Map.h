#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

using namespace sf;

class Stage{
    public:
        std::shared_ptr<Stage> next = nullptr;
        bool is_drawn = false;
        std::unique_ptr<Sprite> stageSprite = nullptr;
};

class Map{
    private:
        std::string stageSpriteName = "player.png";
        std::shared_ptr<Stage> currentStage = nullptr;
        std::shared_ptr<Stage> stageList = nullptr;
        std::shared_ptr<Stage> lastStage = nullptr;
        std::unique_ptr<Sprite> getSprite(std::string);
    public:
        Map();
        void drawMap(std::shared_ptr<sf::RenderWindow>);
        void addStage();
        Vector2f getCurrentStagePos();
        void traverseRight();
};

#endif
