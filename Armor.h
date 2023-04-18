//
// Created by xkapp on 18.04.2023.
//

#ifndef ZOOLS2023_XKADERK2_ARMOR_H
#define ZOOLS2023_XKADERK2_ARMOR_H

#include "Item.h"

namespace Entities {
    class Armor : public Item {
        int m_armorValue;

    public:

        Armor(std::string name, std::string info, int armorValue);

        int getArmorValue();
    };
}






#endif //ZOOLS2023_XKADERK2_ARMOR_H