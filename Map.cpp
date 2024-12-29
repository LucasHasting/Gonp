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
    currentStage->stageNumber = ++max_stage_number;
    stageList->stageSprite = getSprite(stageSpriteName);
}

void Map::drawMap(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<Stage> stageTraversal, char previousDirection){
    switch(previousDirection){
        case 'N':
            if(stageTraversal->up != nullptr){
                drawMap(window, stageTraversal->up, 'N');
            }
            if(stageTraversal->right != nullptr){
                drawMap(window, stageTraversal->right, 'E');
            }
            if(stageTraversal->left != nullptr){
                drawMap(window, stageTraversal->left, 'W');
            }
            break;
        case 'E':
            if(stageTraversal->up != nullptr){
                drawMap(window, stageTraversal->up, 'N');
            }
            if(stageTraversal->right != nullptr){
                drawMap(window, stageTraversal->right, 'E');
            }
            if(stageTraversal->down != nullptr){
                drawMap(window, stageTraversal->down, 'S');
            }
            break;
        case 'S':
            if(stageTraversal->right != nullptr){
                drawMap(window, stageTraversal->right, 'E');
            }
            if(stageTraversal->down != nullptr){
                drawMap(window, stageTraversal->down, 'S');
            }
            if(stageTraversal->left != nullptr){
                drawMap(window, stageTraversal->left, 'W');
            }
            break;
        case 'W':
            if(stageTraversal->up != nullptr){
                drawMap(window, stageTraversal->up, 'N');
            }
            if(stageTraversal->down != nullptr){
                drawMap(window, stageTraversal->down, 'S');
            }
            if(stageTraversal->left != nullptr){
                drawMap(window, stageTraversal->left, 'W');
            }
            break;
        default:
            if(stageTraversal->up != nullptr){
                drawMap(window, stageTraversal->up, 'N');
            }
            if(stageTraversal->right != nullptr){
                drawMap(window, stageTraversal->right, 'E');
            }
            if(stageTraversal->down != nullptr){
                drawMap(window, stageTraversal->down, 'S');
            }
            if(stageTraversal->left != nullptr){
                drawMap(window, stageTraversal->left, 'W');
            };
    }

    window->draw(*(stageTraversal->stageSprite));
}

void Map::traverseRight(){
    if(currentStage->right != nullptr)
        currentStage = currentStage->right;
    return;
}

void Map::traverseUp(){
    if(currentStage->up != nullptr)
        currentStage = currentStage->up;
    return;
}

void Map::traverseDown(){
    if(currentStage->down != nullptr)
        currentStage = currentStage->down;
    return;
}

void Map::traverseLeft(){
    if(currentStage->left != nullptr)
        currentStage = currentStage->left;
    return;
}

Vector2f Map::getCurrentStagePos(){
    return currentStage->stageSprite->getPosition();
}

std::shared_ptr<Stage> Map::getCurrentStage(){
    return currentStage;
}

//add a stage going both directions from stageNumber in the direction
void Map::addStage(int stageNumber, char direction){
    std::shared_ptr<Stage> previousStage = searchStage(currentStage, stageNumber, 'X');
    Vector2f previousPos = previousStage->stageSprite->getPosition();

    switch(direction){
        case 'N':
            if(previousStage->up != nullptr)
                return;
            previousStage->up = std::make_shared<Stage>();
            previousStage->up->down = previousStage;
            previousStage = previousStage->up;
            previousStage->stageNumber = ++max_stage_number;
            previousStage->stageSprite = getSprite(stageSpriteName);
            previousStage->stageSprite->setPosition(previousPos.x, previousPos.y - 60);
            break;
        case 'E':
            if(previousStage->right != nullptr)
                return;
            previousStage->right = std::make_shared<Stage>();
            previousStage->right->left = previousStage;
            previousStage = previousStage->right;
            previousStage->stageNumber = ++max_stage_number;
            previousStage->stageSprite = getSprite(stageSpriteName);
            previousStage->stageSprite->setPosition(previousPos.x + 60, previousPos.y);
            break;
        case 'S':
            if(previousStage->down != nullptr)
                return;
            previousStage->down = std::make_shared<Stage>();
            previousStage->down->up = previousStage;
            previousStage = previousStage->down;
            previousStage->stageNumber = ++max_stage_number;
            previousStage->stageSprite = getSprite(stageSpriteName);
            previousStage->stageSprite->setPosition(previousPos.x, previousPos.y + 60);
            break;
        case 'W':
            if(previousStage->left != nullptr)
                return;
            previousStage->left = std::make_shared<Stage>();
            previousStage->left->right = previousStage;
            previousStage = previousStage->left;
            previousStage->stageNumber = ++max_stage_number;
            previousStage->stageSprite = getSprite(stageSpriteName);
            previousStage->stageSprite->setPosition(previousPos.x - 60, previousPos.y);
            break;
        default:
            break;
    }

    return;
}

std::shared_ptr<Stage> Map::searchStage(std::shared_ptr<Stage> stageTraversal, int stageNumber, char previousDirection){
    std::shared_ptr<Stage> result = nullptr;

    if(stageTraversal->stageNumber == stageNumber){
        return stageTraversal;
    }

    switch(previousDirection){
        case 'N':
            if(stageTraversal->up != nullptr){
                result = searchStage(stageTraversal->up, stageNumber, 'N');
            }
            if(stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result != nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
        case 'E':
            if(stageTraversal->up != nullptr){
                result = searchStage(stageTraversal->up, stageNumber, 'N');
            }
            if(stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result != nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            break;
        case 'S':
            if(stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result != nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            if(result != nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
        case 'W':
            if(stageTraversal->up != nullptr){
                result = searchStage(stageTraversal->up, stageNumber, 'N');
            }
            if(result != nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            if(result != nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
        default:
            if(stageTraversal->up != nullptr){
                result = searchStage(stageTraversal->up, stageNumber, 'N');
            }
            if(stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result != nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            if(result != nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
    }

    if(result != nullptr){
        return result;
    } else {
        return nullptr;
    }
}
