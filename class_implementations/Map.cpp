#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Color.hpp>
#include <memory>
#include <unistd.h>

#include "../class_headers/Map.h"
#include "../class_headers/NewSprite.h"
#include "../class_headers/Stage.h"
#include "../class_headers/GameConstants.h"

using namespace sf;

std::unique_ptr<NewSprite> Map::getSprite(std::string spriteName){
    std::unique_ptr<NewSprite> sprite = std::make_unique<NewSprite>();
    
    //Sprite and Texture declarations
    sprite->texture = std::make_unique<Texture>();
    if(!(sprite->texture->loadFromFile(spriteName))) {
        exit(1);
    }

    sprite->sprite = std::make_unique<Sprite>(*(sprite->texture));

    //return the sprite using the texture
    return sprite;
}

Map::Map(){
    //create the first stage and set the sprite
    stageList = std::make_shared<Stage>();
    currentStage = stageList;
    currentStage->stageNumber = ++max_stage_number;
    currentStage->stageSprite = getSprite(stageSpriteName);
    currentStage->stageTrail = getSprite(stageTrailName);
    currentStage->isDrawn = true;
}

void Map::driver(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<View> camera){
    //will be replaced with animations later
    bool should_sleep = false;

    //move
    if (Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        traverseRight();
        should_sleep = true;
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        traverseUp();
        should_sleep = true;
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        traverseDown();
        should_sleep = true;
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        traverseLeft();
        should_sleep = true;
    }

    if (Keyboard::isKeyPressed(sf::Keyboard::Z))
    {
        drawSurrounding();
        should_sleep = true;
    }

    //set position of camera
    camera->setCenter(getCurrentStagePos());
    window->setView(*camera);

    //draw the current screen
    drawMap(window, getCurrentStage(), 'X');

    //display the screen
    window->display();

    if(should_sleep){
        usleep(0.2 * MICRO);
    }
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

    if(stageTraversal->isDrawn){
        //draw each sprite is it can be drawn
        window->draw(*(stageTraversal->stageSprite->sprite));

        if(stageTraversal->stageNumber != 1)
            window->draw(*(stageTraversal->stageTrail->sprite));
    }else{
        //draw each sprite at lower opacity if it cannot be drawn
        Color colorA(stageTraversal->stageSprite->sprite->getColor());
        Color colorB(stageTraversal->stageTrail->sprite->getColor());

        colorA.a = 155;
        colorB.a = 155;

        stageTraversal->stageSprite->sprite->setColor(colorA);
        stageTraversal->stageTrail->sprite->setColor(colorB);
        
        window->draw(*(stageTraversal->stageSprite->sprite));
        window->draw(*(stageTraversal->stageTrail->sprite));

        //set the color back to original opacity
        colorA.a = 255;
        colorB.a = 255;
        
        stageTraversal->stageSprite->sprite->setColor(colorA);
        stageTraversal->stageTrail->sprite->setColor(colorB);
    }
}

//go right if a connection exists
void Map::traverseRight(){
    if(currentStage->right != nullptr && currentStage->right->isDrawn)
        currentStage = currentStage->right;
    return;
}

//go up if a connection exists
void Map::traverseUp(){
    if(currentStage->up != nullptr && currentStage->up->isDrawn)
        currentStage = currentStage->up;
    return;
}

//go down if a connection exists
void Map::traverseDown(){
    if(currentStage->down != nullptr && currentStage->down->isDrawn)
        currentStage = currentStage->down;
    return;
}

//go left if a connection exists
void Map::traverseLeft(){
    if(currentStage->left != nullptr && currentStage->left->isDrawn)
        currentStage = currentStage->left;
    return;
}

//get the current stage position
Vector2f Map::getCurrentStagePos(){
    return currentStage->stageSprite->sprite->getPosition();
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
    Vector2f previousPos = previousStage->stageSprite->sprite->getPosition();

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
            previousStage->stageTrail = getSprite(stageTrailName);

            previousStage->stageSprite->sprite->setPosition(previousPos.x, previousPos.y - SPRITE_SHIFT);
            previousStage->stageTrail->sprite->setPosition(previousPos.x, previousPos.y - TRAIL_SHIFT);
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
            previousStage->stageTrail = getSprite(stageTrailName);
            previousStage->stageSprite->sprite->setPosition(previousPos.x + SPRITE_SHIFT, previousPos.y);
            previousStage->stageTrail->sprite->setPosition(previousPos.x + TRAIL_SHIFT, previousPos.y);
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
            previousStage->stageTrail = getSprite(stageTrailName);
            previousStage->stageSprite->sprite->setPosition(previousPos.x, previousPos.y + SPRITE_SHIFT);
            previousStage->stageTrail->sprite->setPosition(previousPos.x, previousPos.y + TRAIL_SHIFT);
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
            previousStage->stageTrail = getSprite(stageTrailName);
            previousStage->stageSprite->sprite->setPosition(previousPos.x - SPRITE_SHIFT, previousPos.y);
            previousStage->stageTrail->sprite->setPosition(previousPos.x - TRAIL_SHIFT, previousPos.y);
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
