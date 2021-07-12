#ifndef KEYBOARDCONTROLCOMPONENT_H
#define KEYBOARDCONTROLCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "../Component.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"

class KeyboardControlComponent: public Component {
    public:
        std::string upKey;
        std::string downKey;
        std::string rightKey;
        std::string leftKey;
        std::string shootKey;

        TransformComponent *transform;
        SpriteComponent *sprite;

        KeyboardControlComponent() {

        }

        KeyboardControlComponent(
            std::string upKey, std::string downKey,
            std::string rightKey, std::string leftKey, 
            std::string shootKey) {

                this->upKey = GetSDLKeyStringCode(upKey);
                this->rightKey = GetSDLKeyStringCode(rightKey);
                this->downKey = GetSDLKeyStringCode(downKey);
                this->leftKey = GetSDLKeyStringCode(leftKey);
                this->shootKey = GetSDLKeyStringCode(shootKey);
        }

        void Initialize() override {
            transform = owner->GetComponent<TransformComponent>();
            sprite = owner->GetComponent<SpriteComponent>();
        }

        void Update(float deltaTime) override {            
            if (Game::event.type == SDL_KEYDOWN) {
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);

                if (keyCode.compare(this->upKey) == 0) {
                    transform->velocity.y = -70;
                    transform->velocity.x = 0;
                    sprite->Play("UpAnimation");
                } else if (keyCode.compare(this->downKey) == 0) {
                    transform->velocity.y = 70;
                    transform->velocity.x = 0;
                    sprite->Play("DownAnimation");
                } else if (keyCode.compare(this->leftKey) == 0) {
                    transform->velocity.y = 0;
                    transform->velocity.x = -70;
                    sprite->Play("LeftAnimation");
                } else if (keyCode.compare(this->rightKey) == 0) {
                    transform->velocity.y = 0;
                    transform->velocity.x = 70;
                    sprite->Play("RightAnimation");
                } else if (keyCode.compare(this->shootKey) == 0) {
                    // TODO:
                    // Shoot projectiles when 'shoot' key is pressed
                }
            } else if (Game::event.type == SDL_KEYUP) {
                std::string keyCode = std::to_string(Game::event.key.keysym.sym);

                if (keyCode.compare(this->upKey) == 0) {
                    transform->velocity.y = 0;
                    sprite->Play("UpAnimation");
                } else if (keyCode.compare(this->downKey) == 0) {
                    transform->velocity.y = 0;
                    sprite->Play("DownAnimation");
                } else if (keyCode.compare(this->leftKey) == 0) {
                    transform->velocity.x = 0;
                    sprite->Play("LeftAnimation");
                } else if (keyCode.compare(this->rightKey) == 0) {
                    transform->velocity.x = 0;
                    sprite->Play("RightAnimation");
                } else if (keyCode.compare(this->shootKey) == 0) {
                    // TODO:..
                }
            }
        }

        std::string GetSDLKeyStringCode(std::string key) {
            if (key.compare("up") == 0) return "1073741906";
            if (key.compare("down") == 0) return "1073741905";
            if (key.compare("left") == 0) return "1073741904";
            if (key.compare("right") == 0) return "1073741903";
            if (key.compare("space") == 0) return "32";
            return std::to_string(static_cast<int>(key[0]));
        }


};


#endif