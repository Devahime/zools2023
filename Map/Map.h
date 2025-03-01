//
// Created by xinsanic on 27.04.2023.
//

#ifndef ZOOLS2023_XKADERK2_MAP_H
#define ZOOLS2023_XKADERK2_MAP_H
#include <string>
#include <vector>
#include "Tile.h"
#include "Point.h"

namespace Map{
    class Map {
    private:
        std::string m_mapName;
        std::vector<std::vector<Tile*>> m_tiles;
        Point* m_entryPoint;
        Point* m_exitPoint;
        std::vector<Tile*> m_tileEntitites;
    public:
        Map(std::vector<std::vector<Tile*>> tiles, std::vector<Tile*> tileEntities, Point* entryPoint, Point* exitPoint);

        ~Map();

        Tile* getTile(int x, int y);

        void replaceTile(int x, int y, Tile* tile);

        void swapTiles(int xOrigin, int yOrigin, int xDestination, int yDestination);

        void print();

        Point* getEntryPoint();

        Point* getExitPoint();

        bool isItem(int x, int y);

        bool isEnemy(int x, int y);

        bool isNotWall(int x, int y);

        bool isDoor(int x, int y);

    };
}

#endif //ZOOLS2023_XKADERK2_MAP_H
