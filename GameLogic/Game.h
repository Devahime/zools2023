//
// Created by xkapp on 29.04.2023.
//

#ifndef ZOOLS2023_XKADERK2_GAME_H
#define ZOOLS2023_XKADERK2_GAME_H


//#include "../Map/Level.h"
#include "../Player/Player.h"
#include "Combat.h"
#include "../Entitites/Item.h"


namespace GameLogic {
    class Game {
        //Map::Level* m_level;
        Player::Player* m_player;
        int m_currentMap;
        GameLogic::Combat* m_combat;
    public:
        Game(Player::Player* player, GameLogic::Combat* combat/*, Map::Level* level*/);

        bool checkForAction(char input);

        void clearScreen();

        void printMap();

        void InventoryGUI();

        void printPlayer();

        void combatGUI(Entities::Enemy* enemy);

        bool combat();

        void printAbilityOverview();

    };
}





#endif //ZOOLS2023_XKADERK2_GAME_H