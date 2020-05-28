#include "window.hpp"

#include <iostream>
#include <variant>

#include "config.hpp"
#include "event.hpp"
#include "utils.hpp"

void
glfwErrorCallback(int code, char const* description)
{
    std::cerr << "Error code: " << code << "\nDescription: " << description
              << '\n';
}

GLFWwindow*
createWindow(Config const& conf)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    return glfwCreateWindow(
            conf.get<Settings::WindowWidth>(),
            conf.get<Settings::WindowHeight>(),
            "MandelLandscape",
            nullptr,
            nullptr);
}

Window::Window(Config const& conf) : m_window(createWindow(conf))
{
    if(m_window == nullptr) {
        std::cerr << "Error: GLFWwindow was not created\n";
        throw;
    }

    glfwMakeContextCurrent(m_window.get());

    if(gladLoadGL() == 0) {
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

    return glfwWindowShouldClose(m_window.get()) == 0;
}

void
Window::handleEvent(const Event& event)
{
    std::visit(
            util::Overload{
                    [this](KeyDown key) {
                        if(key.code == GLFW_KEY_Q) {
                            close();
                        }
                    },

                    // default
                    util::unaryNOP},
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
Window::cursorPositionCB(GLFWwindow* glfwWindow, double x, double y)
{
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    auto dx              = x - window->m_lastMouseX;
    auto dy              = y - window->m_lastMouseY;
    window->m_lastMouseX = x;
    window->m_lastMouseY = y;
    window->registerEvent(MouseMove{x, y, dx, dy});
}

void
Window::keyboardCB(
        GLFWwindow* glfwWindow,
        int key,
        [[maybe_unused]] int scancode,
        int action,
        [[maybe_unused]] int mods)
{
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    switch(action) {
    case GLFW_PRESS: {
        window->registerEvent(KeyDown{key, mods});
    } break;

    case GLFW_RELEASE: {
        window->registerEvent(KeyUp{key, mods});
    } break;
    }
}

void
Window::mouseButtonCB(
        GLFWwindow* glfwWindow,
        int button,
        int action,
        [[maybe_unused]] int mods)
{
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    switch(action) {
    case GLFW_PRESS: {
        window->registerEvent(MouseButtonDown{button});
    } break;
    case GLFW_RELEASE: {
        window->registerEvent(MouseButtonUp{button});
    }
    }
}
