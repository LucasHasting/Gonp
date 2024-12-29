#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

#include "Map.h"

using namespace sf;

std::unique_ptr<Sprite> Map::getSprite(std::string spriteName){
    //Sprite and Texture declarations
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    if(!(texture->loadFromFile(spriteName))) {
        exit(1);
    }

    std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>(*texture);
    return sprite;
}

Map::Map(){
    stageList = std::make_shared<Stage>();
    currentStage = stageList;
    lastStage = currentStage;
    stageList->stageSprite = getSprite(stageSpriteName);
}

void Map::drawMap(std::shared_ptr<sf::RenderWindow> window){
    std::shared_ptr<Stage> stageTraveral = currentStage;
    window->draw(*(stageTraveral->stageSprite));

    while(stageTraveral->next != nullptr){
        stageTraveral = stageTraveral->next;
        window->draw(*(stageTraveral->stageSprite));
    }
}

void Map::traverseRight(){
    if(currentStage->next != nullptr)
        currentStage = currentStage->next;
    return;
}

Vector2f Map::getCurrentStagePos(){
    return currentStage->stageSprite->getPosition();
}

void Map::addStage(){
    Vector2f previousPos = lastStage->stageSprite->getPosition();
    lastStage->next = std::make_shared<Stage>();
    lastStage = lastStage->next;
    lastStage->stageSprite = getSprite(stageSpriteName);
    lastStage->stageSprite->setPosition(previousPos.x + 60, previousPos.y);
    return;
}

