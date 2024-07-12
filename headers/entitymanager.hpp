#pragma once
#include "entity.hpp"
#include <unordered_map>
#include <vector>
#include <iostream>

using EntityIDMap = std::unordered_map<size_t, Entity *>;
using EntityTypeMap = std::unordered_map<ENTITY_TYPE, std::vector<size_t>>;

class EntityManager2
{
private:
    EntityIDMap m_entityMap;
    EntityTypeMap m_entityTagList;
    size_t m_totalEntities;

    void destroyID(size_t id);

public:
    EntityManager2();
    ~EntityManager2();

    const size_t createEntity(ENTITY_TYPE tag);
    Entity *getEntityByID(size_t id);
    void update();
    const std::vector<size_t> &getEntities(ENTITY_TYPE tag) const;

    // Just for Rule of 5
    EntityManager2(const EntityManager2 &) = delete;
    EntityManager2 &operator=(const EntityManager2 &) = delete;
    EntityManager2(EntityManager2 &&) = delete;
    EntityManager2 &operator=(EntityManager2 &&) = delete;
};