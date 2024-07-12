#include "renderer.hpp"
#include "window.hpp"
#include "entitymanager.hpp"
#include <random>

int main(int argc, char *argv[])
{
    int vMin, vMax;
    float radius;
    const int SCR_WIDTH = 1024;
    const int SCR_HEIGHT = 768;

    std::cin >> vMin >> vMax >> radius;
    if (vMin < 3 || vMin > vMax || radius <= 0.0f)
    {
        std::cerr << "Initial arguments are invalid.\n";
        exit(-1);
    }

    Window mainWin{SCR_WIDTH, SCR_HEIGHT};
    EntityManager2 manager{};

    if (!mainWin.window)
    {
        std::cout << "Window failed to initialize!\n";
        exit(-1);
    }

    std::string pathToVert = "shaders/polyplace.glsl";
    std::string pathToFrag = "shaders/polyfill.glsl";
    std::string pathToOutlineFrag = "shaders/polyoutline.glsl";

    Renderer simpleRenderer{vMin, vMax, radius, SCR_WIDTH, SCR_HEIGHT, pathToVert, pathToFrag, pathToOutlineFrag};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(vMin, vMax);
    int randVert = dis(gen);

    double lastTime = glfwGetTime();
    double currTime;

    while (!glfwWindowShouldClose(mainWin.window))
    {
        currTime = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (currTime - lastTime >= 2)
        {
            lastTime = currTime;
            randVert = dis(gen);
        }

        simpleRenderer.Draw(3, glm::vec3(0.2f, 0.5f, 0.3f), glm::vec2(300, 100), -50 * glm::radians(currTime));
        simpleRenderer.Draw(randVert, glm::vec3(0.1f, 0.6f, 0.2f), glm::vec2(300, 120), 200 * glm::radians(currTime));
        glfwSwapBuffers(mainWin.window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}