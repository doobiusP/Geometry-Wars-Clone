#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    Window(const float windowW, const float windowH);
    Window() = default;

    GLFWwindow *window;

private:
    GLFWwindow *initializeWindow(const float windowW, const float windowH);
};