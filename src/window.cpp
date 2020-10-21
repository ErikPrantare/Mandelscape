#include "window.hpp"

#include <iostream>
#include <variant>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
createWindow(glm::ivec2 const size)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

    glfwSetErrorCallback(&glfwErrorCallback);

    return glfwCreateWindow(
            size.x,
            size.y,
            "MandelLandscape",
            nullptr,
            nullptr);
}

Window::Window(glm::ivec2 const size) :
            m_window(createWindow(size)),
            m_size(size)
{
    if(m_window == nullptr) {
        std::cerr << "GLFW window was not created\n";
        throw;
    }

    glfwMakeContextCurrent(m_window.get());

    if(gladLoadGL() == 0) {
        std::cerr << "glad failed to initialize\n";
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
    glfwSetWindowSizeCallback(m_window.get(), &resizeCB);
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

auto
Window::handleMomentaryAction(MomentaryAction const& action) -> void
{
    if(sameAction(action, TriggerAction::TogglePause)) {
        m_paused = !m_paused;
        if(m_paused) {
            glfwSetInputMode(m_window.get(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else {
            glfwSetInputMode(
                    m_window.get(),
                    GLFW_CURSOR,
                    GLFW_CURSOR_DISABLED);
        }
    }
    if(sameAction(action, TriggerAction::TakeScreenshot)) {
        screenshot();
    }
    if(sameAction(action, TriggerAction::CloseWindow)) {
        close();
    }
}

auto
Window::paused() -> bool
{
    return m_paused;
}

void
Window::close()
{
    glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
}

void
Window::screenshot()
{
    /*
    glfwMakeContextCurrent(m_window.get());
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    int x        = viewport[0];
    int y        = viewport[1];
    int width    = viewport[2];
    int height   = viewport[3];
    char* pixels = new char[3 * width * height];
    glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    stbi_write_png("test.png", width, height, 3, pixels, 0);
    delete[] pixels;
    */
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
        int /*scancode*/,
        int action,
        int mods)
{
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    switch(action) {
    case GLFW_PRESS: {
        window->registerEvent(KeyDown{(Input::Key)key, mods});
    } break;

    case GLFW_RELEASE: {
        window->registerEvent(KeyUp{(Input::Key)key, mods});
    } break;
    }
}

void
Window::mouseButtonCB(
        GLFWwindow* glfwWindow,
        int button,
        int action,
        int /*mods*/)
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

void
Window::resizeCB(GLFWwindow* glfwWindow, int width, int height)
{
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    glViewport(0, 0, width, height);
    window->m_size = {width, height};
}
