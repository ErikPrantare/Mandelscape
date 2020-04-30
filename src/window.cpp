#include "window.h"

#include <iostream>
#include <variant>

#include "config.h"
#include "event.h"
#include "utils.h"

GLFWwindow*
createWindow(Config const& conf)
{
    glfwInit();
    glfwWindowHint(GLFW_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    return glfwCreateWindow(
            conf.get<Settings::WindowWidth>(),
            conf.get<Settings::WindowHeight>(),
            "MandelLandscape",
            nullptr,
            nullptr);
}

Window::Window(Config const& conf) : m_window(createWindow(conf))
{
    if(!m_window.get()) {
        std::cerr << "Error: GLFWwindow was not created\n";
        throw;
    }

    glfwMakeContextCurrent(m_window.get());

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Something went wrong!\n";
        throw;
    }

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    setCallbacks();

    glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetInputMode(m_window.get(), GLFW_STICKY_KEYS, GLFW_FALSE);
}

void
Window::setCallbacks()
{
    glfwSetWindowUserPointer(m_window.get(), static_cast<void*>(this));

    glfwSetCursorPosCallback(m_window.get(), &cursorPositionCB);
    glfwSetKeyCallback(m_window.get(), &keyboardCB);
    glfwSetMouseButtonCallback(m_window.get(), &mouseButtonCB);
}

std::optional<Event>
Window::nextEvent()
{
    return util::pop(m_events);
}

bool
Window::update()
{
    glfwSwapBuffers(m_window.get());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    return !glfwWindowShouldClose(m_window.get());
}

void
Window::handleEvent(const Event& event)
{
    std::visit(
            util::overload{
                    [this](KeyDown key) {
                        if(key.key == GLFW_KEY_Q)
                            close();
                    },

                    // default
                    [](auto x) -> Require<!SpecialisationOf<
                                       decltype(x),
                                       std::variant>::value> {
                    }},
            event);
}

void
Window::close()
{
    glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
}

void
Window::registerEvent(Event&& event)
{
    m_events.emplace(std::forward<Event>(event));
}

void
Window::cursorPositionCB(GLFWwindow* window, double x, double y)
{
    auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

    _this->registerEvent(MouseMove{x, y});
}

void
Window::keyboardCB(
        GLFWwindow* window,
        int key,
        int scancode,
        int action,
        int mods)
{
    auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

    switch(action) {
    case GLFW_PRESS: {
        _this->registerEvent(KeyDown{key, mods});
    } break;

    case GLFW_RELEASE: {
        _this->registerEvent(KeyUp{key, mods});
    } break;
    }
}

void
Window::mouseButtonCB(GLFWwindow* window, int button, int action, int mods)
{
    auto _this = static_cast<Window*>(glfwGetWindowUserPointer(window));

    switch(action) {
    case GLFW_PRESS: {
        _this->registerEvent(MouseButtonDown{button});
    } break;
    case GLFW_RELEASE: {
        _this->registerEvent(MouseButtonUp{button});
    }
    }
}
