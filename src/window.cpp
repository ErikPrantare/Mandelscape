#include "window.h"

#include <iostream>
#include <variant>

#include "config.h"
#include "event.h"
#include "utils.h"

void
registerKeyDown(unsigned char, int, int);
void
registerKeyUp(unsigned char, int, int);
void
registerMouseMove(int, int);
void
registerMouseButton(int, int, int, int);

Window* windowInstance;

Window::Window(Config const& conf)
{
    windowInstance = this;

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(
            conf.get<Settings::WindowWidth>(),
            conf.get<Settings::WindowHeight>());
    glutInitWindowPosition(100, 100);
    glutCreateWindow("MandelLandscape");
    glutSetKeyRepeat(false);
    glutSetCursor(GLUT_CURSOR_NONE);

    GLenum res = glewInit();
    if(res != GLEW_OK) {
        std::cerr << "Error: " << glewGetErrorString(res) << std::endl;
        throw;
    }

    std::cout << "GL version: " << glGetString(GL_VERSION) << std::endl;

    glClearColor(1, 1, 1, 0);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearDepth(10'000'000.0f);

    glutKeyboardFunc(registerKeyDown);
    glutKeyboardUpFunc(registerKeyUp);
    glutPassiveMotionFunc(registerMouseMove);
    glutMouseFunc(registerMouseButton);
}

std::optional<Event>
Window::nextEvent()
{
    return util::pop(m_events);
}

void
registerEvent(Event&& event)
{
    windowInstance->m_events.push(std::forward<Event>(event));
}

void
registerKeyDown(unsigned char code, int, int)
{
    registerEvent(KeyDown{.code = code});
}

void
registerKeyUp(unsigned char code, int, int)
{
    registerEvent(KeyUp{.code = code});
}

void
registerMouseMove(int x, int y)
{
    registerEvent(MouseMove{.x = x, .y = y});
}

void
registerMouseButton(int button, int state, int, int)
{
    switch(state) {
    case GLUT_DOWN: {
        registerEvent(MouseButtonDown{.button = button});
    } break;

    case GLUT_UP: {
        registerEvent(MouseButtonUp{.button = button});
    } break;
    }
}
