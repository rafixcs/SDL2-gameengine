#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "./TransformComponent.h"

class SpriteComponent : public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRect;
        SDL_Rect dstRect;
    public:
        
        SpriteComponent(const char* filePath);
        ~SpriteComponent();

        void SetTexture(std::string assetTextureId);
        void Initialize() override;
        void Update(float deltaTime) override;
        void Render() override;


    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

};

#endif