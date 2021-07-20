#include "./Map.h"
#include "./Game.h"
#include "./EntityManager.h"
#include "./Components/TileComponent.h"

#include <fstream>

extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize) {
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y=0; y < mapSizeY; y++) {
        for (int x=0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int sourceRecY = atoi(&ch) * tileSize;

            mapFile.get(ch);            
            int sourceRecX = atoi(&ch) * tileSize;

            AddTile(sourceRecX, sourceRecY, x * this->scale * this->tileSize, y * this->scale * this->tileSize);

            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::AddTile(int sourceX, int sourceY, int x, int y) {
    Entity& newTile(manager.AddEntity("Tile", TILEMAP_LAYER));
    newTile.AddComponent<TileComponent>(sourceX, sourceY, x, y, this->tileSize, this->scale, this->textureId);
}