#include "entitymanager.hpp"

EntityManager::~EntityManager()
{
    for (const auto &e : m_entitiesToAdd)
    {
        delete e;
    }

    m_entitiesToAdd.clear();
    m_entityMap.clear();

    for (const auto &e : m_entities)
    {
        delete e;
    }
    m_entities.clear();
}

void EntityManager::deferredUpdate()
{
    for (const auto &e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->getTag()].push_back(e);
    }

    m_entitiesToAdd.clear();

    for (auto &eVecPair : m_entityMap)
    {
        EntityVec &eVec = eVecPair.second;
        std::remove_if(eVec.begin(), eVec.end(), [](Entity *e)
                       { return !e->isActive(); });
    }

    std::vector<Entity *> temp;
    for (const auto &e : m_entities)
    {
        if (!e->isActive())
            temp.push_back(e);
    }
    std::remove_if(m_entities.begin(), m_entities.end(), [](Entity *e)
                   { return !e->isActive(); });

    for (Entity *e : temp)
    {
        delete e;
    }
}

Entity *EntityManager::addEntity(ENTITY_TYPE tag)
{
    Entity *e = new Entity(++m_totalEntities, tag);
    m_entitiesToAdd.push_back(e);
    return e;
}

const EntityVec &EntityManager::getEntities() const
{
    return m_entities;
}

const EntityVec &EntityManager::getEntities(ENTITY_TYPE tag) const
{
    return m_entityMap.at(tag);
}
