#ifndef MANDELLANDSCAPE_WINDOW_H
#define MANDELLANDSCAPE_WINDOW_H

#include <queue>
#include <optional>
#include <memory>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "config.h"
#include "event.h"

class Window {
public:
    Window(Config const& conf);

    Window(Window&&) = default;

    Window&
    operator=(Window&&) = default;

    Window(Window const&) = delete;

    Window&
    operator=(Window const&) = delete;

    GLFWwindow*
    operator()()
    {
        return m_window.get();
    }

    std::optional<Event>
    nextEvent();

private:
    struct WindowDeleter {
        void
        operator()(GLFWwindow* window) noexcept
        {
            glfwDestroyWindow(window);
        }
    };

    std::unique_ptr<GLFWwindow, WindowDeleter> m_window;

    std::queue<Event> m_events;

    void
    registerEvent(Event&& event);
};

#endif
