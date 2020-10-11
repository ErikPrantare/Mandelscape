#ifndef MANDELLANDSCAPE_GLFW_INPUT_HPP
#define MANDELLANDSCAPE_GLFW_INPUT_HPP

#include "glfwEnums.hpp"

#include <variant>
#include <queue>
#include <optional>
#include <tuple>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

namespace Input {

using Peripheral = std::variant<Key, MouseButton>;

struct Button {
    Peripheral code{};
    ButtonState state{};
};

constexpr auto
operator==(Button const& lhs, Button const& rhs) noexcept -> bool
{
    return std::tie(lhs.code, lhs.state) == std::tie(rhs.code, rhs.state);
}

struct CursorPosition {
    glm::dvec2 position;
};

struct Events {
    std::queue<Button> buttons                 = std::queue<Button>();
    std::queue<CursorPosition> cursorPositions = std::queue<CursorPosition>();
};

auto
cursorPositionCB(GLFWwindow* window, double x, double y) -> void;

auto
keyboardCB(
        GLFWwindow* window,
        int key,
        int /*scancode*/,
        int action,
        int /*mods*/) -> void;

auto
mouseButtonCB(GLFWwindow* window, int code, int action, int /*mods*/) -> void;

}    // namespace Input

namespace std {
template<>
struct hash<Input::Button> {
    auto
    operator()(Input::Button const& button) noexcept -> size_t
    {
        using Code  = decay_t<decltype(button.code)>;
        using State = decay_t<decltype(button.state)>;

        return hash<Code>()(button.code) ^ (hash<State>()(button.state) << 1);
    }
};
}    // namespace std

#endif    // MANDELLANDSCAPE_GLFW_INPUT_HPP
