#include "renderer.hpp"
#include <random>

int main(int argc, char *argv[])
{
    int vMin, vMax;
    float radius;
    const int SCR_WIDTH = 640;
    const int SCR_HEIGHT = 480;

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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Hello World", NULL, NULL);
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

    Renderer simpleRenderer = Renderer(vMin, vMax, radius, SCR_WIDTH, SCR_HEIGHT, pathToVert, pathToFrag);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(vMin, vMax);
    int randVert = dis(gen);

    double lastTime = glfwGetTime();
    double currTime;

    // std::cout << randVert << '\n';
    while (!glfwWindowShouldClose(window))
    {
        currTime = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (currTime - lastTime >= 2)
        {
            lastTime = currTime;
            randVert = dis(gen);
            // std::cout << randVert << '\n';
        }

        simpleRenderer.Draw(3, glm::vec3(0.2f, 0.5f, 0.3f), glm::vec2(300, 100), -50 * glm::radians(currTime));
        simpleRenderer.Draw(randVert, glm::vec3(0.1f, 0.6f, 0.2f), glm::vec2(100, 200), 200 * glm::radians(currTime));
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}