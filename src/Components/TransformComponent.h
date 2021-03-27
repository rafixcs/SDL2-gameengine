#ifndef TRANSFORMCOMPONET_H
#define TRANSFORMCOMPONET_H

#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"

class TransformComponent: public Component {
    public:
        glm::vec2 position;
        glm::vec2 velocity;
        int width;
        int height;
        int scale;

        TransformComponent(int posX, int posY, int velX, int velY, int h, int w, int s);

        void Initialize() override;
        void Update(float deltaTime) override;
        void Render() override;


};

#endif