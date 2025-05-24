#pragma once

#include <GLFW/glfw3.h>

class Window {
public:
    Window();
    ~Window();

    bool isOpen();
    void swapBuffers();
    void handleEvents();

    GLFWwindow* getNativeWindow() const { return window; }

private:
    GLFWwindow* window;
};