//
// Created by xkapp on 16.06.2023.
//

#ifndef ZOOLS2023_XKADERK2_GUI_H
#define ZOOLS2023_XKADERK2_GUI_H
#include "vector"
#include "../Entitites/Item.h"
#include "../Player/Player.h"
#include "../Entitites/Enemy.h"
#include "../Map/Map.h"

namespace GameLogic {
    class GUI {

    public:
        GUI();

        void clearScreen();

        void printInventory(std::vector<Entities::Item*>);

        void combatGUI(Player::Player* player, Entities::Enemy* enemy);

        void printGameScreen(Player::Player* player, Map::Map* currentMap);


    };
}



#endif //ZOOLS2023_XKADERK2_GUI_H
