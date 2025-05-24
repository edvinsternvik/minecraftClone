#include "Input.h"
#include "../Application.h"
#include "../misc/Window.h"

bool Input::keyDown(int keycode) {
    auto window = Application::getInstance().getWindow()->getNativeWindow();
    auto keystate = glfwGetKey(window, keycode);
    return keystate == GLFW_PRESS || keystate == GLFW_REPEAT;
}

bool Input::mouseKeyDown(int keycode) {
    auto window = Application::getInstance().getWindow()->getNativeWindow();
    auto keystate = glfwGetMouseButton(window, keycode);
    return keystate == GLFW_PRESS;
}

Vector2 Input::mousePos() {
    auto window = Application::getInstance().getWindow()->getNativeWindow();
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return Vector2(xpos, ypos);
}
