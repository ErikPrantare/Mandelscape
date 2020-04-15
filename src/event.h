#ifndef MANDELLANDSCAPE_EVENT_H
#define MANDELLANDSCAPE_EVENT_H

#include <variant>

struct KeyDown {
    unsigned char code;
};

struct KeyUp {
    unsigned char code;
};

struct MouseMove {
    int x;
    int y;
};

struct MouseButtonDown {
    int button;
};

struct MouseButtonUp {
    int button;
};

using Event =
        std::variant<KeyUp, KeyDown, MouseMove, MouseButtonUp, MouseButtonDown>;

#endif
