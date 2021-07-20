#include <iostream>
#include "./Entity.h"
#include "./Constants.h"

Entity::Entity(EntityManager& manager): manager(manager) {
    this->debugMode = false;
    this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string name, LayerType layer): manager(manager), name(name), layer(layer) {
    this->debugMode = false;
    this->isActive = true;
}

void Entity::Update(float deltaTime) {
    if (this->debugMode) {
        std::cout << "Debug mode for " << this->name << std::endl;
    }
    for (auto& component: components) {
        component->Update(deltaTime);
    }
}

void Entity::Render() {
    for (auto& component: components) {
        component->Render();
    }
}

void Entity::Destroy() {
    this->isActive = false;
}

bool Entity::IsActive() const {
    return this->isActive;
}
