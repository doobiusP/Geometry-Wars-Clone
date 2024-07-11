#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <vector>

using EntityVec = std::vector<Entity *>;
using EntityMap = std::unordered_map<ENTITY_TYPE, EntityVec>;

class EntityManager
{
private:
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;

    EntityMap m_entityMap;
    size_t m_totalEntities = 0;

public:
    EntityManager() = default;
    ~EntityManager();

    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;
    EntityManager(EntityManager &&) = delete;
    EntityManager &operator=(EntityManager &&) = delete;

    void deferredUpdate();

    Entity *addEntity(ENTITY_TYPE tag);

    const EntityVec &getEntities() const;
    const EntityVec &getEntities(ENTITY_TYPE tag) const;
};