#include "window.h"

#include <iostream>

#include "config.h"

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

bool
Window::pollEvent(Event& event)
{
    if(m_events.empty()) {
        return false;
    }

    event = m_events.front();
    m_events.pop();
    return true;
}

void
registerEvent(Event&& event)
{
    windowInstance->m_events.push(std::move(event));
}

void
registerKeyDown(unsigned char code, int, int)
{
    Event event;
    event.type     = Event::KeyDown;
    event.key.code = code;
    registerEvent(std::move(event));
}

void
registerKeyUp(unsigned char code, int, int)
{
    Event event;
    event.type     = Event::KeyUp;
    event.key.code = code;
    registerEvent(std::move(event));
}

void
registerMouseMove(int x, int y)
{
    Event event;
    event.type        = Event::MouseMove;
    event.mouseMove.x = x;
    event.mouseMove.y = y;
    registerEvent(std::move(event));
}

void
registerMouseButton(int button, int state, int, int)
{
    Event event;
    event.type = state == GLUT_DOWN ? Event::MouseDown : Event::MouseUp;
    event.mouseButton.button = button;
    registerEvent(std::move(event));
}
