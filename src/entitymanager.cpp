#include "entitymanager.hpp"

void EntityManager::destroyID(size_t id)
{
    auto &ref = m_entityTagList[m_entityMap[id]->getTag()];
    ref.erase(std::remove(ref.begin(), ref.end(), id), ref.end());

    delete m_entityMap[id];
    m_entityMap.erase(id);
}

EntityManager::EntityManager() : m_totalEntities{0}
{
}

EntityManager::~EntityManager()
{
    m_entityTagList.clear();
    for (auto &e : m_entityMap)
    {
        delete e.second;
    }

    m_entityMap.clear();
}

const size_t EntityManager::createEntity(ENTITY_TYPE tag)
{
    Entity *newEntity{new Entity(++m_totalEntities, tag)};

    m_entityMap[m_totalEntities] = newEntity;
    m_entityTagList[tag].push_back(m_totalEntities);

    return m_totalEntities;
}

Entity *EntityManager::getEntityByID(size_t id)
{
    return m_entityMap.contains(id) ? m_entityMap[id] : nullptr;
}

void EntityManager::update()
{
    std::vector<size_t> idsToDelete;
    for (const auto &e : m_entityMap)
    {
        if (!(e.second->isActive()))
            idsToDelete.push_back(e.first);
    }
    for (auto id : idsToDelete)
    {
        destroyID(id);
    }
}

const std::vector<size_t> &EntityManager::getEntities(ENTITY_TYPE tag) const
{
    return (m_entityTagList.contains(tag) && m_entityTagList.at(tag).size() >= 1) ? m_entityTagList.at(tag) : m_tempReturnList;
}
