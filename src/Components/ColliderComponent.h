#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../Component.h"
#include "../Game.h"
#include "../EntityManager.h"
#include "../Components/TransformComponent.h"

class ColliderComponent: public Component {
    public:
        std::string colliderTag;
        SDL_Rect collider;
        SDL_Rect srcRect;
        SDL_Rect dstRect;
        TransformComponent* transform;
        bool debugMode;

        ColliderComponent(std::string colliderTag, int x, int y, int width, int height) {
            this->colliderTag = colliderTag;
            this->collider = {x, y, width, height};
            this->debugMode = false;
        }

        void Initialize() override {
            if (owner->HasComponent<TransformComponent>()) {
                this->transform = owner->GetComponent<TransformComponent>();
                this->srcRect = {0, 0, this->transform->width, this->transform->height};
                this->dstRect = {collider.x, collider.y, collider.w ,collider.h};
            }
        }

        void Update(float deltaTime) override {
            this->collider.x = static_cast<int>(this->transform->position.x);
            this->collider.y = static_cast<int>(this->transform->position.y);
            this->collider.w = this->transform->width * this->transform->scale;
            this->collider.h = this->transform->height * this->transform->scale;
            this->dstRect.x = this->collider.x - Game::camera.x;
            this->dstRect.y = this->collider.y - Game::camera.y;
        }

        void Render() override {
            if (debugMode) {
                SDL_SetRenderDrawColor(Game::renderer, 255, 255, 0, 255);
                SDL_RenderDrawRect(Game::renderer, &this->dstRect);
            }
        }

};

#endif