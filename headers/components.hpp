#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct CTransform
{
    glm::vec2 pos;
    glm::vec2 vel;
    float angle;

    CTransform(const glm::vec2 &p, const glm::vec2 &v, float a) : pos{p}, vel{v}, angle{a}
    {
    }
};

struct CShape
{
    int numSides;
    glm::vec3 color;

    CShape(int s, const glm::vec3 &c) : numSides{s}, color{c}
    {
    }
};

struct CCollision
{
    float radius;

    CCollision(float r) : radius{r}
    {
    }
};

struct CScore
{
    int score;

    CScore(int s) : score{s}
    {
    }
};

struct CLifespan
{
    int remaining;
    int total;

    CLifespan(int t) : remaining{t}, total{t}
    {
    }
};

struct CInput
{
    bool up = 0;
    bool right = 0;
    bool down = 0;
    bool left = 0;
    bool shoot = 0;

    CInput() = default;
};
