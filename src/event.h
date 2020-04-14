#ifndef MANDELLANDSCAPE_EVENT_H
#define MANDELLANDSCAPE_EVENT_H

struct Event {
    enum EventType { KeyDown, KeyUp, MouseMove, MouseUp, MouseDown };

    struct KeyEvent {
        unsigned char code;
    };

    struct MouseMoveEvent {
        int x;
        int y;
    };

    struct MouseButtonEvent {
        int button;
    };

    EventType type;

    union {
        KeyEvent key;
        MouseMoveEvent mouseMove;
        MouseButtonEvent mouseButton;
    };
};

#endif
