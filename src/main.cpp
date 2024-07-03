#include "renderer.hpp"
#include <random>

int main(int argc, char *argv[])
{
    int vMin, vMax;
    float radius;

    std::cin >> vMin >> vMax >> radius;
    if (vMin < 3 || vMin > vMax || radius <= 0.0f)
    {
        std::cerr << "Initial arguments are invalid.\n";
        exit(-1);
    }

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW failed to initialize!\n";
        exit(-1);
    }

    std::string pathToVert = "shaders/polyplace.glsl";
    std::string pathToFrag = "shaders/polyfill.glsl";

    Renderer simpleRenderer = Renderer(vMin, vMax, radius, pathToVert, pathToFrag);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(vMin, vMax);
    int randVert = dis(gen);
    // int n;
    // float r;
    // std::cin >> n >> r;

    // std::vector<float> pentagon;
    // std::vector<unsigned int> indices;
    // generatePolygon(pentagon, indices, n, r);

    // // for (int i = 0; i < pentagon.size(); ++i)
    // // {
    // //     std::cout << pentagon[i] << ", ";
    // //     if ((i + 1) % 2 == 0)
    // //         std::cout << '\n';
    // // }
    // // std::cout << "\n";
    // // for (int i = 0; i < indices.size(); ++i)
    // // {
    // //     std::cout << indices[i] << ",";
    // //     if ((i + 1) % 3 == 0)
    // //         std::cout << '\n';
    // // }

    // GLuint ebo, vbo, vao;
    // glGenBuffers(1, &ebo);
    // glGenBuffers(1, &vbo);
    // glGenVertexArrays(1, &vao);

    // glBindVertexArray(vao);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    // glBufferData(GL_ARRAY_BUFFER, pentagon.size() * sizeof(float), &pentagon[0], GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
    // glEnableVertexAttribArray(0);

    // Shader simple = Shader("shaders/polyplace.glsl", "shaders/polyfill.glsl");
    // simple.use();
    double lastTime = glfwGetTime();
    double currTime;

    std::cout << randVert << '\n';
    while (!glfwWindowShouldClose(window))
    {
        currTime = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (currTime - lastTime >= 5)
        {
            lastTime = currTime;
            randVert = dis(gen);
            std::cout << randVert << '\n';
        }

        simpleRenderer.Draw(randVert, glm::vec3(0.1f, 0.6f, 0.2f));
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}