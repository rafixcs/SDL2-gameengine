#include "./TransformComponent.h"
#include "../Game.h"
#include <SDL2/SDL.h>

TransformComponent::TransformComponent(int posX, int posY, int velX, int velY, int h, int w, int s) {
    this->position = glm::vec2(posX, posY);
    this->velocity = glm::vec2(velX, velY);
    this->width = w;
    this->height = h;
    this->scale = s;
}

void TransformComponent::Initialize(){

}

void TransformComponent::Update(float deltaTime) {
    this->position.x += this->velocity.x * deltaTime;
    this->position.y += this->velocity.y * deltaTime;    
}

void TransformComponent::Render() {
    SDL_Rect transformRectangle = {
        static_cast<int>(this->position.x),
        static_cast<int>(this->position.y),
        this->width,
        this->height
    };

    SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255); 
    SDL_RenderFillRect(Game::renderer, &transformRectangle);
}