#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "./TransformComponent.h"
#include "../Component.h"

class SpriteComponent : public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRect;
        SDL_Rect dstRect;
    public:
        
        SpriteComponent(const char* filePath) {
            this->SetTexture(filePath);
        }

        void SetTexture(std::string assetTextureId) {
            texture = Game::assetManager->GetTexture(assetTextureId);
        }
        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            this->srcRect.x = 0;
            this->srcRect.y = 0;
            this->srcRect.w = transform->width;
            this->srcRect.h = transform->height;
        }
        void Update(float deltaTime) override {
            this->dstRect.x = static_cast<int>(transform->position.x);
            this->dstRect.y = static_cast<int>(transform->position.y);
            this->dstRect.w = transform->width * transform->scale;
            this->dstRect.h = transform->height * transform->scale;
        }
        void Render() override {
            /* std::cout << "x: " << this->dstRect.x << std::endl; 
            std::cout << "y: " << this->dstRect.y << std::endl;
            std::cout << "w: " << this->dstRect.w << std::endl;
            std::cout << "h: " << this->dstRect.h << std::endl;
            std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";*/

            TextureManager::Draw(this->texture, this->srcRect, this->dstRect, spriteFlip);
        }


    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

};

#endif