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
}

std::optional<Event>
Window::nextEvent()
{
    return util::pop(m_events);
}

void
Window::registerEvent(Event&& event)
{
    m_events.emplace(std::forward<Event>(event));
}
