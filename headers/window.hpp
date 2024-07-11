#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *initializeWindow(const float windowW, const float windowH);

class Window
{
public:
    Window(const float windowW, const float windowH);

    GLFWwindow *window;

private:
};