//
// Created by xkapp on 29.04.2023.
//

#include "Game.h"
#include <iostream>
#include <stdlib.h>
//#define ASCII_ESC 27
#include "../Entitites/ItemType.h"
#include <chrono>
#include <thread>
#include <conio.h>
#include "../Map/Floor.h"
#include "../Map/Wall.h"
#include "../Map/PlayerTile.h"
#include "../Map/Door.h"
#include "../Map/EnemyTile.h"
#include "../Map/ItemTile.h"
#include "../Player/Kick.h"


GameLogic::Game::Game(Player::Player *player, Map::Level* level) {
    m_player = player;
    m_combat = nullptr;
    m_currentMap = 0;
    m_level = level;
    m_gui = new GUI();
    m_gameComplete = false;
}

bool GameLogic::Game::checkForAction(char input) {
    //Could be done by case switch
    //Checking pressedkey (input), then doing action acording to controls
    if (m_gameComplete) {
        return false;
    }

    if (input == 'w') {
        //std::cout << "Nahoru" << std::endl;
        mapMovement(input);
        if (m_gameComplete) {
            return false;
        }
        printGameScreen();
        return true;

    } else if(input == 'a') {
        mapMovement(input);
        if (m_gameComplete) {
            return false;
        }
        printGameScreen();
        return true;

    } else if(input == 's') {
        //std::cout << "Dolu" << std::endl;
        mapMovement(input);
        if (m_gameComplete) {
            return false;
        }
        printGameScreen();
        return true;

    } else if(input == 'd') {
        mapMovement(input);
        if (m_gameComplete) {
            return false;
        }
        //std::cout << "Doprava" << std::endl;
        printGameScreen();
        return true;

    } else if(input == 'x') {
        //exits the game
        //std::cout << "Exit" << std::endl;
        return false;

    } else if(input == 'i') {
        //opens invetory
        InventoryGUI();
        printGameScreen();
        return true;

    } else if (input == 'e') { //debug
        auto abilitites = m_player->getAbilities();
        for (int i = 0; i < abilitites.size(); ++i) {
            std::cout << abilitites[i]->getName() << std::endl;
        }
        return true;

    } /*else if (input == 'c') { //debug
        if (combat()){
            return true;
        } else {
            return false;
        }

    }*/ else {
        //if the keypressed is not found in this if tree, map is refreshed
        printGameScreen();
    }
}


void GameLogic::Game::printMap() {
    Map::Map* map = m_level->getMap(m_currentMap);
    map->print();
}

void GameLogic::Game::clearScreen() {
    m_gui->clearScreen();
}

/*void GameLogic::Game::printPlayer() { //debug
    std::cout << m_player->getName();
}*/



bool GameLogic::Game::combat(Entities::Enemy* enemy) {
    bool combat = true;
    //auto enemy = new Entities::Enemy("skeleton", 80, 10, 5); //debug
    m_combat = new Combat();
    m_combat->beginCombat(m_player,enemy); //debug

    while (combat) {
        combatGUI(enemy);

        if (m_combat->checkAliveStatus() != "bothAlive") {
            combat = false;
        }
        /*if (m_combat->isPlayersTurn()) {

        } else {
            m_combat->nextTurn();
        }*/
        clearScreen();
    }

    if (m_combat->checkAliveStatus() == "playerDead") {
        delete m_combat;
        m_combat = nullptr;
        return false;

    } else if (m_combat->checkAliveStatus() == "enemyDead") {
        delete m_combat;
        m_combat = nullptr;
        return true;
    }
}



void GameLogic::Game::InventoryGUI() {
    auto PlayerInventory = m_player->getPlayerInvenotry();
    auto inventory = PlayerInventory->getAllItems();
    m_gui->printInventory(inventory, m_player);

    std::string input;
    std::cin >> input;

    //if players types q, inventoryGUI will finish and the player will be brought back to gamescreen/combat
    if (input=="q") {
        //continue without action -> quit
    } else if (std::stoi(input) >=1 and std::stoi(input) <= inventory.size()+1) {
        clearScreen();
       // Entities::Item* pickedItem = inventory[input]; nefunguje
       int pickedItemIndex = std::stoi(input) -1;

        if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::armor) {
            if (inventory[pickedItemIndex] == PlayerInventory->getEquippedArmor()) {
                m_gui->printArmorInfo(static_cast<Entities::Armor*>(inventory[pickedItemIndex]), true);
            } else {
                m_gui->printArmorInfo(static_cast<Entities::Armor*>(inventory[pickedItemIndex]), false);
            }


        } else if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::relic) {
            if (inventory[pickedItemIndex] == PlayerInventory->getEquippedRelic()) {
                m_gui->printRelicInfo(static_cast<Entities::Relic*>(inventory[pickedItemIndex]), true);
            } else {
                m_gui->printRelicInfo(static_cast<Entities::Relic*>(inventory[pickedItemIndex]), false);
            }


        } else if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::weapon) {
            if (inventory[pickedItemIndex] == PlayerInventory->getEquippedWeapon()) {
                m_gui->printWeaponInfo(static_cast<Entities::Weapon*>(inventory[pickedItemIndex]), true);
            } else {
                m_gui->printWeaponInfo(static_cast<Entities::Weapon*>(inventory[pickedItemIndex]), false);
            }


        } else if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::consumable) {
            m_gui->printConsumableInfo(static_cast<Entities::Consumable*>(inventory[pickedItemIndex]));
        }

        //player can "use" the item whci means equip/unequip or consume (consumable)
        std::cout << "\nEnter 'u' to use or equip item or any other letter to exit: ";
        std::cin >> input;

        if (input == "u") {

            if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::consumable) {
                m_player->useReplenishment(static_cast<Entities::Consumable*>(inventory[pickedItemIndex]), pickedItemIndex);
            } else {
                if (inventory[pickedItemIndex] == PlayerInventory->getEquippedWeapon()) {
                    m_player->dropWeapon();
                } else if (inventory[pickedItemIndex] == PlayerInventory->getEquippedRelic()) {
                    m_player->dropRelic();
                } else if (inventory[pickedItemIndex] == PlayerInventory->getEquippedArmor()) {
                    m_player->dropArmor();
                } else {
                    m_player->equipItem(pickedItemIndex);
                }
            }
            /*if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::consumable) {
               m_player->useReplenishment(static_cast<Entities::Consumable*>(inventory[pickedItemIndex]));
               m_player->deleteItemFromInvenotry(pickedItemIndex);


            } else if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::weapon) {
                if (inventory[pickedItemIndex] == m_player->getEquippedWeapon()) {
                    m_player->dropWeapon();
                } else {
                    m_player->dropWeapon();
                    m_player->equipWeapon(static_cast<Entities::Weapon*>(inventory[pickedItemIndex]));
                }


            } else if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::armor){
                if (inventory[pickedItemIndex] == m_player->getEquippedArmor()) {
                    m_player->dropArmor();
                } else {
                    m_player->dropArmor();
                    m_player->equipWeapon(static_cast<Entities::Weapon*>(inventory[pickedItemIndex]));
                }


            } else if (inventory[pickedItemIndex]->getItemType() == Entities::ItemType::relic) {
                if (inventory[pickedItemIndex] == m_player->getEquippedRelic()) {
                    m_player->dropRelic();
                } else {
                    m_player->dropRelic();
                    m_player->equipRelic(static_cast<Entities::Relic*>(inventory[pickedItemIndex]));
                }
            }*/
        }
    }
    //after this the screen will be cleared so next event ifnromation can be shown
    clearScreen();

}




void GameLogic::Game::combatGUI(Entities::Enemy *enemy) {
    std::string choice;

    //checking if player has turn, if so, player can then cast ability or check inventory
    if (m_combat->isPlayersTurn()) {
        m_gui->combatScreen(m_player, enemy, true);
        std::cin >> choice;

        if (choice=="i") {
            InventoryGUI();
            //if player chooses inventory, he still has turn, and once the combat gui will be called again, he can cast ability (or enter
            //inventory again)
        } else if (std::stoi(choice) > 0 and std::stoi(choice) <= m_player->getAbilities().size()) {
            auto ability = m_player->getAbility(std::stoi(choice)-1);
            if (ability->getName() =="Punch") {
                enemy->takeDamage(ability->doDamage(0,m_player->getStrenght()));
                m_combat->nextTurn();
            } else if (ability->getName() == "Kick") {
                if (static_cast<Player::Kick*>(ability)->getCooldownTimer() == 0) {
                    enemy->takeDamage((static_cast<Player::Kick*>(ability)->doDamage(0, m_player->getStrenght())));
                    m_combat->nextTurn();
                } else {
                    std::cout << "There is a cooldown on this ability in this turn! Use another one." << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(3));
                }

            } else {
                //slash
                enemy->takeDamage(ability->doDamage(m_player->getPlayerInvenotry()->getWeaponDamage() ,m_player->getStrenght()));
                m_combat->nextTurn();
            }
            //if player chose ability, then enemy has the turn
        }

    } else {
        //enemy turn
        m_gui->combatScreen(m_player, enemy, false);
        m_player->takeDamage(m_combat->enemyDamageFromAction());
        //once enemy finished casting his ability, player again will have turn in next function calling
        m_combat->nextTurn();
    }




}


void GameLogic::Game::printTutorial() {
    std::string input;

    m_gui->printTutorial(m_player);
    input = _getch();
    clearScreen();

}

bool GameLogic::Game::isPlayerAlive() {
    return m_player->isAlive();
}


void GameLogic::Game::printGameScreen() {
    std::cout << "\n" << "      Room " << m_currentMap+1 << std::endl;
    std::cout << "\n";

    printMap();
    std::cout << "\n";

    std::cout << "    Player: " << m_player->getName() <<
    "  [" << m_player->getHealth() << "/" << m_player->getMaxHealth() << "]\n" <<std::endl;

    std::cout << "        Controls:" << std::endl
              << "     w,a,s,d - Movement" << std::endl
              << "     x - Exit game" << std::endl
              << "     i - Inventory\n" << std::endl;
}




//map movement fucntions

bool isNotWall(int x, int y, Map::Map* map){
    if (map->getTile(x,y)->getType() != Map::TileType::WallType) {
        return true;
    } else {
        return false;
    }
};

bool isDoor(int x, int y, Map::Map* map){
    if (map->getTile(x,y)->getType() == Map::TileType::DoorType) {
        return true;
    } else {
        return false;
    }
}

bool isEnemy(int x, int y, Map::Map* map) {
    if (map->getTile(x, y)->getType() == Map::TileType::EnemyTileType) {
        return true;
    } else {
        return false;
    }
}

bool isItem(int x, int y, Map::Map* map) {
    if (map->getTile(x, y)->getType() == Map::TileType::Item) {
        return true;
    } else {
        return false;
    }
}


void GameLogic::Game::mapMovement(char pressedKey) {

    Map::Point *cordinates = m_player->getPlayerPosition();
    auto map = m_level->getMap(m_currentMap);

    int xCordinate = cordinates->x;
    int yCordinate = cordinates->y;

    int xNewCordiante;
    int yNewCordinate;

    if (pressedKey == 'w') {
        xNewCordiante = xCordinate;
        yNewCordinate = yCordinate - 1;

    } else if (pressedKey == 'a') {
        xNewCordiante = xCordinate - 1;
        yNewCordinate = yCordinate;

    } else if (pressedKey == 's') {
        xNewCordiante = xCordinate;
        yNewCordinate = yCordinate + 1;

    } else if (pressedKey == 'd') {
        xNewCordiante = xCordinate + 1;
        yNewCordinate = yCordinate;
    }

    if (isNotWall(xNewCordiante, yNewCordinate, map)) {
        //check if target tile is door
        if (isDoor(xNewCordiante, yNewCordinate, map)) {
            //cehck if the door is exit or entry one
            if (static_cast<Map::Door *>(map->getTile(xNewCordiante, yNewCordinate))->isExitDoor()) {
                m_currentMap = static_cast<Map::Door *>(map->getTile(xNewCordiante, yNewCordinate))->getTargetRoom();
                if (m_currentMap+1>m_level->getLevelSize()) {
                    completeTheGame();
                } else {
                    newMapSetup(true);
                }
            } else {
                m_currentMap = static_cast<Map::Door *>(map->getTile(xNewCordiante, yNewCordinate))->getTargetRoom();
                newMapSetup(false);
            }

        } else if (isEnemy(xNewCordiante, yNewCordinate, map)) {
            Entities::Enemy *enemy = static_cast<Map::EnemyTile *>(map->getTile(xNewCordiante,
                                                                                yNewCordinate))->getEnemy();
            bool combatResult = combat(enemy);
            if (combatResult) {
                auto item = enemy->dropItem();

                if (item != nullptr) {
                    map->replaceTile(xNewCordiante, yNewCordinate, new Map::ItemTile(item));
                } else {
                    map->replaceTile(xNewCordiante, yNewCordinate, new Map::Floor());
                }
            }


        } else if (isItem(xNewCordiante, yNewCordinate, map)) {
            auto item = static_cast<Map::ItemTile *>(map->getTile(xNewCordiante, yNewCordinate))->takeItem();
            m_player->addItemToInventory(item);
            map->swapTiles(xCordinate, yCordinate, xNewCordiante, yNewCordinate);
            m_player->changePlayerPosition(xNewCordiante, yNewCordinate);
            //already swapped tile, need to go back cordinate wise
            map->replaceTile(xCordinate, yCordinate, new Map::Floor());


        } else {
            map->swapTiles(xCordinate, yCordinate, xNewCordiante, yNewCordinate);
            m_player->changePlayerPosition(xNewCordiante, yNewCordinate);
        }
    }

}




void GameLogic::Game::newMapSetup(bool isExitDoor) {
    auto map = m_level->getMap(m_currentMap);

    if (isExitDoor) {
        auto point = map->getEntryPoint();
        m_player->changePlayerPosition(point->x, point->y);
        map->replaceTile(point->x, point->y, new Map::PlayerTile());

    } else {
        auto point = map->getExitPoint();
        m_player->changePlayerPosition(point->x, point->y);
        map->replaceTile(point->x, point->y, new Map::PlayerTile());
    }

}

bool GameLogic::Game::isGameCompleted() {
    return m_gameComplete;
}

void GameLogic::Game::completeTheGame() {
    char input;
    m_gui->gameCompletionScreen(m_player->getName());
    m_gameComplete = true;
    input = _getch();
}