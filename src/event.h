#ifndef MANDELLANDSCAPE_EVENT_H
#define MANDELLANDSCAPE_EVENT_H

#include <variant>

struct KeyDown {
    int const code;
    int const mods;
};

struct KeyUp {
    int const code;
    int const mods;
};

struct MouseMove {
    double const x;
    double const y;
};

struct MouseButtonDown {
    int const button;
};

struct MouseButtonUp {
    int const button;
};

using Event =
        std::variant<KeyUp, KeyDown, MouseMove, MouseButtonUp, MouseButtonDown>;

#endif
