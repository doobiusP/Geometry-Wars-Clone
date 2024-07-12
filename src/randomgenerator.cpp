#include "randomgenerator.hpp"

RandomGenerator::RandomGenerator(int windowW, int windowH, int standardRadius, int vMin, int vMax)
    : m_gen(std::random_device{}()),
      m_distX(standardRadius, windowW - standardRadius),
      m_distY(standardRadius, windowH - standardRadius),
      m_vCount(vMin, vMax),
      m_distColor(0, 255)
{
}

glm::vec2 RandomGenerator::generateCoordinates()
{
    int x = m_distX(m_gen);
    int y = m_distY(m_gen);
    return {x, y};
}

int RandomGenerator::generateVertex()
{
    return m_vCount(m_gen);
}

glm::vec4 RandomGenerator::generateColor()
{
    float r = m_distColor(m_gen);
    float g = m_distColor(m_gen);
    float b = m_distColor(m_gen);
    return glm::vec4(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}
