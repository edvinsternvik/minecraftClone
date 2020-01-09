#include "Window.h"

Window::Window() {
	if (!glfwInit())
        return;

    window = glfwCreateWindow(1280, 720, "MC", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);

	glfwSwapInterval(0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

Window::~Window() {
	glfwTerminate();
}

bool Window::isOpen() {
	return !glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}

void Window::handleEvents() {
	glfwPollEvents();
}