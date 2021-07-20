#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H

#include <SDL2/SDL.h>
#include "../Component.h"
#include "../EntityManager.h"
#include "../AssetManager.h"
#include "../../lib/glm/glm.hpp"

class TileComponent: public Component {
    public:
        SDL_Texture *texture;
        SDL_Rect sourceRectangle;
        SDL_Rect destinyRectangle;
        glm::vec2 position;

        TileComponent(int sourceRectX, int sourceRectY, int x, int y, int tileSize, int tileScale, std::string assetTextureId) {
            this->texture = Game::assetManager->GetTexture(assetTextureId);
            this->sourceRectangle.x = sourceRectX;
            this->sourceRectangle.y = sourceRectY;
            this->sourceRectangle.w = tileSize;
            this->sourceRectangle.h = tileSize;

            this->destinyRectangle.x = x;
            this->destinyRectangle.y = y;
            this->destinyRectangle.w = tileSize * tileScale;
            this->destinyRectangle.h = tileSize * tileScale;

            position.x = x;
            position.y = y;

        }

        ~TileComponent() {

        }

        void Update(float deltaTime) override {
            this->destinyRectangle.x = position.x - Game::camera.x;
            this->destinyRectangle.y = position.y - Game::camera.y;
        }

        void Render() override {
            TextureManager::Draw(texture, sourceRectangle, destinyRectangle, SDL_FLIP_NONE);
        }


};

#endif