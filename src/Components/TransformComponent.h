#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SDL2/SDL.h>
#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"
#include "../Game.h"

class TransformComponent: public Component {
    public:
        glm::vec2 position;
        glm::vec2 velocity;
        int width;
        int height;
        int scale;

        TransformComponent(int posX, int posY, int velX, int velY, int h, int w, int s) {
            this->position = glm::vec2(posX, posY);
            this->velocity = glm::vec2(velX, velY);
            this->width = w;
            this->height = h;
            this->scale = s;
        }

        void Initialize() override {

        }
        void Update(float deltaTime) override {
            this->position.x += this->velocity.x * deltaTime;
            this->position.y += this->velocity.y * deltaTime;
        }
        void Render() override {
               
        }


};

#endif