#pragma once
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RandomGenerator
{
private:
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_distX;
    std::uniform_int_distribution<> m_distY;
    std::uniform_int_distribution<> m_vCount;
    std::uniform_int_distribution<> m_distColor;

public:
    RandomGenerator() = default;
    RandomGenerator(int windowW, int windowH, int standardRadius, int vMin, int vMax);

    glm::vec2 generateCoordinates();
    int generateVertex();
    glm::vec4 generateColor();
};