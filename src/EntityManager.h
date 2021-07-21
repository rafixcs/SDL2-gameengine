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

    public:
        Entity& AddEntity(std::string entityName, LayerType layer);
    
    public:
        void ClearData();
        void DestroyInactiveEntity();        
        
    public:
        unsigned int GetEntityCount();
        std::vector<Entity*> GetEntities() const;
        std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
    
    public:    
        CollisionType CheckCollisions() const;


};

#endif