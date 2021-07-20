#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "./Entity.h"
#include "./Constants.h"

class EntityManager {
    private:
        std::vector<Entity*> entities;
    public:
        void Update(float deltaTime);
        void Render();
        bool HasNoEntities();
        Entity& AddEntity(std::string entityName, LayerType layer);
        void ClearData();
        std::vector<Entity*> GetEntities() const;
        std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
        unsigned int GetEntityCount();
        CollisionType CheckCollisions() const;

};

#endif