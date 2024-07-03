#include "renderer.hpp"

void Renderer::fillPolygonData(int polyIndex)
{
    int n = m_vStart + polyIndex;
    float r = m_radius;
    std::vector<float> &polyN = m_polyData[polyIndex].polyVertices;
    std::vector<unsigned int> &polyInd = m_polyData[polyIndex].polyIndices;

    const float PI = glm::pi<float>();
    float angleStep = 2.0f * PI / n;

    // Generate vertices for the n-sided polygon
    for (int i = 0; i < n; ++i)
    {
        float angle = i * angleStep;
        float x = r * cos(angle);
        float y = r * sin(angle);
        polyN.push_back(x);
        polyN.push_back(y);
    }

    // Generate the triangles
    for (int i = 0; i < n; ++i)
    {
        int currIndex = i;
        int nextIndex = (i + 1) % n;

        polyInd.push_back(n); // (0,0)
        polyInd.push_back(currIndex);
        polyInd.push_back(nextIndex);
    }

    polyN.push_back(0.0f);
    polyN.push_back(0.0f);
}

void Renderer::printPolygonData(int polyIndex)
{
    if (polyIndex < 0 || polyIndex >= m_vLength)
    {
        std::cout << "polyIndex out of bounds...\n";
        return;
    }
    std::cout << "(m_vStart = " << m_vStart << ", m_vLength = " << m_vLength << ", m_radius = " << m_radius << ")\n";
    int n = m_polyData[polyIndex].polyVertices.size();
    int nI = m_polyData[polyIndex].polyIndices.size();

    for (int i = 0; i < n; ++i)
    {
        std::cout << m_polyData[polyIndex].polyVertices[i] << " ";
        if ((i + 1) % 2 == 0)
            std::cout << '\n';
    }
    for (int i = 0; i < nI; ++i)
    {
        std::cout << m_polyData[polyIndex].polyIndices[i] << " ";
        if ((i + 1) % 3 == 0)
            std::cout << '\n';
    }
    std::cout << "---------------------------------------------------------------\n";
}

Renderer::Renderer(int vMin, int vMax, float radius, const std::string &polyVertShader, const std::string &polyFragShader) : m_vStart{vMin}, m_vLength{vMax - vMin + 1}, m_radius{radius}, m_shader(polyVertShader, polyFragShader)
{
    m_polyData.resize(m_vLength);
    for (int i = 0; i < m_vLength; ++i)
    {
        fillPolygonData(i);
    }

    m_lastPolyShape = -1;

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_EBO);
    glGenBuffers(1, &m_VBO);

    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    m_shader.use();
}

void Renderer::Draw(int vCount, glm::vec3 polyColor)
{
    if (vCount < m_vStart || vCount >= m_vStart + m_vLength)
        return;
    vCount -= m_vStart;

    m_shader.setVec3("u_polyColor", polyColor);

    if (m_lastPolyShape != vCount)
    {
        glBufferData(GL_ARRAY_BUFFER, m_polyData[vCount].polyVertices.size() * sizeof(float), &m_polyData[vCount].polyVertices[0], GL_STATIC_DRAW);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_polyData[vCount].polyIndices.size() * sizeof(unsigned int), &m_polyData[vCount].polyIndices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        m_lastPolyShape = vCount;
    }

    glDrawElements(GL_TRIANGLES, m_polyData[vCount].polyIndices.size(), GL_UNSIGNED_INT, (void *)0);
}
