#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <boost/type_index.hpp>
#include "./Component.h"
#include "./EntityManager.h"

#define PRINT_NEWCOMPONENT 0

class EntityManager;

class Entity {
    private:
        EntityManager& manager;
        bool isActive;
        std::vector<Component*> components;
        std::map<const std::type_info*, Component*> componentTypeMap;
    public:
        std::string name;
        Entity(EntityManager& manager);
        Entity(EntityManager& manager, std::string name);
        void Update(float deltaTime);
        void Render();
        void Destroy();
        bool IsActive() const;

        template <typename T, typename... TArgs>
        T& AddComponent(TArgs&&... args) {
            T* newComponent(new T(std::forward<TArgs>(args)...));
            
            newComponent->owner = this;
            
            components.emplace_back(newComponent);
            this->componentTypeMap[&typeid(*newComponent)] = newComponent;
            
            newComponent->Initialize();
            
            #if PRINT_NEWCOMPONENT
                PrintComponentName(newComponent);
            #endif
            
            return *newComponent;
        }

        template <typename T>
        T* GetComponent() {
            return static_cast<T*>(componentTypeMap[&typeid(T)]);
        }
        
        template <typename T>
        void PrintComponentName(T component) {
            std::cout << "Entity: " << this->name << std::endl;
            std::cout << "\tNew component <" << boost::typeindex::type_id<T>().pretty_name() << ">" << std::endl;
        }

        template <typename T>
        bool HasComponent() const {
            return componentTypeMap.count(&typeid(T));
        }

};

#endif ENTITY_H