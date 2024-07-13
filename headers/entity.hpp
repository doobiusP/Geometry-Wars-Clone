#pragma once
#include "components.hpp"
#include <optional>

enum class ENTITY_TYPE
{
    DEFAULT,
    PLAYER,
    BULLET,
    ENEMY
};

class Entity
{
private:
    friend class EntityManager;

    const size_t m_id;
    ENTITY_TYPE m_tag;
    bool m_isAlive;
    Entity(const size_t id, const ENTITY_TYPE tag);

public:
    std::optional<CTransform> cTransform;
    std::optional<CShape> cShape;
    std::optional<CCollision> cCollision;
    std::optional<CScore> cScore;
    std::optional<CLifespan> cLifespan;
    std::optional<CInput> cInput;

    void addTransform(const glm::vec2 &p, const glm::vec2 &v, float a);
    void addShape(int s, float sc, const glm::vec4 &c, const glm::vec4 &oc);
    void addCollision(float r);
    void addScore(int s);
    void addLifeSpan(float t);
    void addInput();

    const size_t getId() const;
    const ENTITY_TYPE getTag() const;
    bool isActive() const;

    void destroy();
};