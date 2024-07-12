#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>

using EntityIDMap = std::unordered_map<size_t, Entity *>;
using EntityTypeMap = std::unordered_map<ENTITY_TYPE, std::vector<size_t>>;

class EntityManager
{
private:
    EntityIDMap m_entityMap;
    EntityTypeMap m_entityTagList;
    size_t m_totalEntities;

    const std::vector<size_t> m_tempReturnList;

    void destroyID(size_t id);

public:
    EntityManager();
    ~EntityManager();

    const size_t createEntity(ENTITY_TYPE tag);
    Entity *getEntityByID(size_t id);
    void update();
    const std::vector<size_t> &getEntities(ENTITY_TYPE tag) const;

    // Just for Rule of 5
    EntityManager(const EntityManager &) = delete;
    EntityManager &operator=(const EntityManager &) = delete;
    EntityManager(EntityManager &&) = delete;
    EntityManager &operator=(EntityManager &&) = delete;
};