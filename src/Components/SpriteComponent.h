#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL2/SDL.h>
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "./TransformComponent.h"
#include "../Component.h"
#include "../Animation.h"

class SpriteComponent : public Component {
    private:
        TransformComponent* transform;
        SDL_Texture* texture;
        SDL_Rect srcRect;
        SDL_Rect dstRect;
        
        int numFrames;
        int animationSpeed;
        bool isFixed;
        bool isAnimated;
        bool hasDirection;
        unsigned int animationIndex;        

        std::map<std::string, Animation> animations;
        std::string currentAnimationName;
    
    public:
        SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
    
    public:
        
        SpriteComponent(const char* filePath) {
            this->animationIndex = 0;
            this->isAnimated = false;
            this->isFixed = false;
            this->hasDirection = false;
            this->animationSpeed = 0;
            this->SetTexture(filePath);
        }

        SpriteComponent(const char* filePath, int numFrames, int animationSpeed, bool hasDirection, bool isFixed) {
            this->animationIndex = 0;
            this->isAnimated = true;
            this->isFixed = isFixed;
            this->animationSpeed = animationSpeed;
            this->hasDirection = hasDirection;

            if (this->hasDirection) {
                Animation downAnimation = Animation(0, numFrames, animationSpeed);
                Animation rightAnimation = Animation(1, numFrames, animationSpeed);
                Animation leftAnimation = Animation(2, numFrames, animationSpeed);
                Animation upAnimation = Animation(3, numFrames, animationSpeed);

                animations.emplace("DownAnimation", downAnimation);
                animations.emplace("RightAnimation", rightAnimation);
                animations.emplace("LeftAnimation", leftAnimation);
                animations.emplace("UpAnimation", upAnimation);

                this->animationIndex = 0;
                this->currentAnimationName = "DownAnimation";
            } else {
                Animation singleAnimation = Animation(0, numFrames, animationSpeed);
                this->animations.emplace("SingleAnimation", singleAnimation);
                this->animationIndex = 0;
                this->currentAnimationName = "SingleAnimation";
            }

            Play(this->currentAnimationName);            
            this->SetTexture(std::string(filePath));
        }        

        void Play(std::string animationName) {
            this->numFrames = animations[animationName].numFrames;
            this->animationIndex = animations[animationName].index;
            this->animationSpeed = animations[animationName].animationSpeed;
            this->currentAnimationName = animationName;
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
            if (this->isAnimated) {
                try {
                    if(owner->debugMode) {
                        std::cout << "Number of frames " << this->numFrames << std::endl;
                    }
                    this->srcRect.x = this->srcRect.w * static_cast<int>((SDL_GetTicks() / this->animationSpeed) % this->numFrames);
                }
                catch (const std::exception& e) {
                    std::cerr << "Update texture error: " << e.what() << std::endl;
                }
            }
            //std::cout << "src x: " << this->srcRect.x << "\tspeed: " << this->animationSpeed  << "\tnumFrames: " << this->numFrames << std::endl;
            this->srcRect.y = this->animationIndex * this->transform->height;

            this->dstRect.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
            this->dstRect.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
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

        SDL_Rect GetSrcRectangle() {
            return this->srcRect;
        }
};

#endif