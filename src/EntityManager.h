#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include "./Entity.h"

class EntityManager {
    private:
        std::vector<Entity*> entities;
    public:
        void Update(float deltaTime);
        void Render();
        bool HasNoEntities();
        Entity& AddEntity(std::string entityName);
        void ClearData();
        std::vector<Entity*> GetEntities() const;
        unsigned int GetEntityCount();      

};

#endif