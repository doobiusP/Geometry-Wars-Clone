#include "entity.hpp"

Entity::Entity(const size_t id, const ENTITY_TYPE tag) : m_id{id}, m_tag{tag}, m_isAlive{true}
{
}

const size_t Entity::getId() const
{
    return m_id;
}

const ENTITY_TYPE Entity::getTag() const
{
    return m_tag;
}

bool Entity::isActive() const
{
    return m_isAlive;
}

void Entity::destroy()
{
    m_isAlive = false;
}
