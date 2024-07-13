#include "entity.hpp"

Entity::Entity(const size_t id, const ENTITY_TYPE tag) : m_id{id}, m_tag{tag}, m_isAlive{true}
{
}

void Entity::addTransform(const glm::vec2 &p, const glm::vec2 &v, float a)
{
    cTransform.emplace(p, v, a);
}

void Entity::addShape(int s, float sc, const glm::vec4 &c, const glm::vec4 &oc)
{
    cShape.emplace(s, sc, c, oc);
}

void Entity::addCollision(float r)
{
    cCollision.emplace(r);
}

void Entity::addScore(int s)
{
    cScore.emplace(s);
}

void Entity::addLifeSpan(float t)
{
    cLifespan.emplace(t);
}

void Entity::addInput()
{
    cInput.emplace();
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