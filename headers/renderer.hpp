#pragma once
#include <vector>

#include "shaderclass.hpp"

struct Polygon
{
    std::vector<float> polyVertices;
    std::vector<unsigned int> polyIndices;
};

class Renderer
{
private:
    const int m_vStart;
    const int m_vLength;
    const float m_radius;
    const int m_windowW;
    const int m_windowH;

    GLuint m_VAO, m_EBO, m_VBO;

    std::vector<Polygon> m_polyData;
    int m_lastPolyShape;

    Shader m_shader;

    void fillPolygonData(int polyIndex);
    void printPolygonData(int polyIndex);

public:
    Renderer(int vMin, int vMax, float radius, int windowW, int windowH, const std::string &polyVertShader, const std::string &polyFragShader);

    void Draw(int vCount, glm::vec3 polyColor);
};
