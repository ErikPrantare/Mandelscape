#ifndef MANDELLANDSCAPE_EVENT_HPP
#define MANDELLANDSCAPE_EVENT_HPP

#include <variant>

#include "glfwEnums.hpp"

struct KeyDown {
    Input::Key const code;
    int const mods;
};

struct KeyUp {
    Input::Key const code;
    int const mods;
};

struct MouseMove {
    double const x;
    double const y;
    double const dx;
    double const dy;
};

struct MouseButtonDown {
    Input::MouseButton const button;
};

struct MouseButtonUp {
    Input::MouseButton const button;
};

using Event = std::
        variant<KeyUp, KeyDown, MouseMove, MouseButtonUp, MouseButtonDown>;

auto
operator<(MouseMove a, MouseMove b) -> bool;
auto
operator<(MouseButtonUp a, MouseButtonUp b) -> bool;
auto
operator<(MouseButtonDown a, MouseButtonDown b) -> bool;

#endif
