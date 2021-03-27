#include "./Entity.h"

Entity::Entity(EntityManager& manager): manager(manager) {
    this->isActive == true;
}

Entity::Entity(EntityManager& manager, std::string name): manager(manager), name(name) {
    this->isActive == true;
}

void Entity::Update(float deltaTime) {
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
    this->isActive == false;
}

bool Entity::IsActive() const {
    return this->isActive;
}

template <typename T, typename... TArgs>
T& Entity::AddComponent(TArgs&&... args) {
    T* newComponent(new T(std::forward<TArgs>(args)...));
    newComponent->owner = this;
    components.emplace_back(newComponent);
    this->componentTypeMap[&typeid(*newComponent)] = newComponent;
    newComponent->Intialize();
    return *newComponent;
}


template <typename T>
T* Entity::GetComponent() {
    return static_cast<T*>(componentTypeMap[&typeid(T)]);
}