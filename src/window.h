#ifndef MANDELLANDSCAPE_WINDOW_H
#define MANDELLANDSCAPE_WINDOW_H

#include <queue>

#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "config.h"
#include "event.h"

class Window {
public:
    Window(Config const& conf);

    Window(Window const&) = delete;
    Window(Window&&)      = delete;
    Window&
    operator=(Window const&) = delete;
    Window&
    operator=(Window&&) = delete;

    bool
    pollEvent(Event&);

private:
    std::queue<Event> m_events;

    friend void
    registerEvent(Event&& event);
};

#endif
