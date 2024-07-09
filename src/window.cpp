#include "window.hpp"

GLFWwindow *initializeWindow(const float windowW, const float windowH)
{
    if (!glfwInit())
        return nullptr;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow *window = glfwCreateWindow(windowW, windowH, "Geometry Wars Clone", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK)
    {
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    return window;
}
