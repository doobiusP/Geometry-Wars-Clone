#include "window.hpp"

GLFWwindow *Window::initializeWindow(const float windowW, const float windowH)
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return window;
}

Window::Window(const float windowW, const float windowH)
{
    window = initializeWindow(windowW, windowH);
}
