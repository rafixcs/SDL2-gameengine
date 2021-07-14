#ifndef WINDOWCOLLIDER_H
#define WINDOWCOLLIDER_H

#include "../Component.h"
#include "../Game.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"

class WindowCollider: public Component {
    private:
        TransformComponent *transform;
        SpriteComponent *sprite;

    public:
        WindowCollider() {

        }

        void Initialize() override {
            this->transform = owner->GetComponent<TransformComponent>();
            this->sprite = owner->GetComponent<SpriteComponent>();
        }

        void Update(float deltaTime) {
            int pos_xmax = transform->position.x + transform->width;
            int pos_ymax = transform->position.y + transform->height;

            if (pos_xmax > WINDOW_WIDTH) {
                transform->position.x = transform->position.x - (pos_xmax - WINDOW_WIDTH);
            }
            if (pos_ymax > WINDOW_HEIGHT) {
                transform->position.y = transform->position.y - (pos_ymax - WINDOW_HEIGHT);
            }
            if(transform->position.x < 0) {   
                transform->position.x = 0;
            }
            if (transform->position.y < 0) {
                transform->position.y = 0;
            }

        }

};

#endif