#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

#include "../class_headers/Map.h"

using namespace sf;

std::unique_ptr<Sprite> Map::getSprite(std::string spriteName){
    //Sprite and Texture declarations
    std::unique_ptr<Texture> texture = std::make_unique<Texture>();
    if(!(texture->loadFromFile(spriteName))) {
        exit(1);
    }

    std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>(*texture);

    //return the sprite using the texture
    return sprite;
}

Map::Map(){
    //create the first stage and set the sprite
    stageList = std::make_shared<Stage>();
    currentStage = stageList;
    currentStage->stageNumber = ++max_stage_number;
    currentStage->stageSprite = getSprite(stageSpriteName);
    currentStage->isDrawn = true;
}

void Map::drawMap(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<Stage> stageTraversal, char previousDirection){
    //traverse the map
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

    if(stageTraversal->isDrawn)
        //draw each sprite is it can be drawn
        window->draw(*(stageTraversal->stageSprite));
}

//go right if a connection exists
void Map::traverseRight(){
    if(currentStage->right != nullptr)
        currentStage = currentStage->right;
    return;
}

//go up if a connection exists
void Map::traverseUp(){
    if(currentStage->up != nullptr)
        currentStage = currentStage->up;
    return;
}

//go down if a connection exists
void Map::traverseDown(){
    if(currentStage->down != nullptr)
        currentStage = currentStage->down;
    return;
}

//go left if a connection exists
void Map::traverseLeft(){
    if(currentStage->left != nullptr)
        currentStage = currentStage->left;
    return;
}

//get the current stage position
Vector2f Map::getCurrentStagePos(){
    return currentStage->stageSprite->getPosition();
}

//get the current stage
std::shared_ptr<Stage> Map::getCurrentStage(){
    return currentStage;
}

void Map::drawSurrounding(){
    if(currentStage->up != nullptr && !currentStage->up->isDrawn){
        currentStage->up->isDrawn = true;
    }
    if(currentStage->down != nullptr && !currentStage->down->isDrawn){
        currentStage->down->isDrawn = true;
    }
    if(currentStage->left != nullptr && !currentStage->left->isDrawn){
        currentStage->left->isDrawn = true;
    }
    if(currentStage->right != nullptr && !currentStage->right->isDrawn){
        currentStage->right->isDrawn = true;
    }
}

//add a stage going both directions from stageNumber in the direction passed in if possible
void Map::addStage(int stageNumber, char direction){
    //find the stage at stageNumber
    std::shared_ptr<Stage> previousStage = searchStage(currentStage, stageNumber, 'X');
    
    //if the stage does not exist, return
    if(previousStage == nullptr)
        return;
    
    //get the position of stage at stageNumber
    Vector2f previousPos = previousStage->stageSprite->getPosition();

    //add the stage based on direction, see N for comments on each case
    switch(direction){
        case 'N':
            //if connection exists, return
            if(previousStage->up != nullptr)
                return;

            //add new stage
            previousStage->up = std::make_shared<Stage>();

            //set the opposite direction's edge (bi-directional)
            previousStage->up->down = previousStage;

            //go to the next stage
            previousStage = previousStage->up;

            //add the stage number
            previousStage->stageNumber = ++max_stage_number;
            
            //get the sprite and set it's position
            previousStage->stageSprite = getSprite(stageSpriteName);
            previousStage->stageSprite->setPosition(previousPos.x, previousPos.y - 60);
            
            break;
        case 'E':
            //same as N
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
            //same as N
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
            //same as N
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
    //if the current stage is the stageNumber, return that stage pointer
    if(stageTraversal->stageNumber == stageNumber){
        return stageTraversal;
    }

    //create result to store the result
    std::shared_ptr<Stage> result = nullptr;

    //traverse the graph
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
            if(result == nullptr && stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result == nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            break;
        case 'S':
            if(stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result == nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            if(result == nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
        case 'W':
            if(stageTraversal->up != nullptr){
                result = searchStage(stageTraversal->up, stageNumber, 'N');
            }
            if(result == nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            if(result == nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
        default:
            if(stageTraversal->up != nullptr){
                result = searchStage(stageTraversal->up, stageNumber, 'N');
            }
            if(result == nullptr && stageTraversal->right != nullptr){
                result = searchStage(stageTraversal->right, stageNumber, 'E');
            }
            if(result == nullptr && stageTraversal->down != nullptr){
                result = searchStage(stageTraversal->down, stageNumber, 'S');
            }
            if(result == nullptr && stageTraversal->left != nullptr){
                result = searchStage(stageTraversal->left, stageNumber, 'W');
            }
            break;
    }

    //return the result
    return result;
}
