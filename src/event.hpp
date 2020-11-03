#ifndef MANDELLANDSCAPE_EVENT_HPP
#define MANDELLANDSCAPE_EVENT_HPP

#include <variant>

#include "glfwEnums.hpp"

struct KeyDown {
    Input::Key code;
    int const mods;
};

struct KeyUp {
    Input::Key code;
    int const mods;
};

struct MouseMove {
    double const x;
    double const y;
    double const dx;
    double const dy;
};

struct MouseButtonDown {
    int const button;
};

struct MouseButtonUp {
    int const button;
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
