#ifndef PROJECTILEEMITTERCOMPONENT_H
#define PROJECTILEEMITTERCOMPONENT_H

#include "../Component.h"
#include "../Components/TransformComponent.h"
#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"

class ProjectileEmitterComponent: public Component {
    private:
        TransformComponent* transform;
        glm::vec2 origin;
        int speed;
        int range;
        float angleRad;
        bool shouldLoop;
    public:
        ProjectileEmitterComponent(int speed, int angle, int range, bool shouldLoop) {
            this->speed = speed;
            this->range = range;
            this->shouldLoop = shouldLoop;
            this->angleRad = glm::radians(static_cast<float>(angle));
        }

        void Initialize() override {
            this->transform = owner->GetComponent<TransformComponent>();
            this->origin = glm::vec2(this->transform->position.x, this->transform->position.y);
            this->transform->velocity = glm::vec2(glm::cos(this->angleRad) * speed, glm::sin(this->angleRad) * speed);
        }

        void Update(float deltaTime) override {
            if (glm::distance(this->transform->position, this->origin) > range) {
                if (this->shouldLoop) {
                    this->transform->position.x = this->origin.x;
                    this->transform->position.y = this->origin.y;
                } else {
                    owner->Destroy();
                }
            }
        }
};

#endif