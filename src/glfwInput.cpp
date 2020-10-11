#include "glfwInput.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <iostream>

namespace Input {

auto
cursorPositionCB(GLFWwindow* window, double x, double y) -> void
{
    auto* eventsPtr = glfwGetWindowUserPointer(window);
    if(eventsPtr == nullptr) {
        throw std::logic_error(
                "GLFWwindow user pointer is not set, "
                "attach an Events struct\n");
    }

    auto&& events = *static_cast<Events*>(eventsPtr);

    events.cursorPositions.push(CursorPosition{{x, y}});
}

auto
keyboardCB(
        GLFWwindow* window,
        int key,
        int /*scancode*/,
        int action,
        int /*mods*/) -> void
{
    auto* eventsPtr = glfwGetWindowUserPointer(window);
    if(eventsPtr == nullptr) {
        throw std::logic_error(
                "GLFWwindow user pointer is not set, "
                "attach an Events struct\n");
    }

    auto&& events = *static_cast<Events*>(eventsPtr);

    events.buttons.push(
            Button{static_cast<MouseButton>(key),
                   static_cast<ButtonState>(action)});
}

auto
mouseButtonCB(GLFWwindow* window, int code, int action, int /*mods*/) -> void
{
    auto* eventsPtr = glfwGetWindowUserPointer(window);
    if(eventsPtr == nullptr) {
        throw std::logic_error(
                "GLFWwindow user pointer is not set, "
                "attach an Events struct\n");
    }

    auto&& events = *static_cast<Events*>(eventsPtr);

    events.buttons.push(
            Button{static_cast<MouseButton>(code),
                   static_cast<ButtonState>(action)});
}

}    // namespace Input
