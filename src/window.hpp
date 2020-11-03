#ifndef MANDELLANDSCAPE_WINDOW_HPP
#define MANDELLANDSCAPE_WINDOW_HPP

#include <queue>
#include <optional>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "event.hpp"
#include "momentaryAction.hpp"

class Window {
public:
    Window(glm::ivec2 size);

    Window()  = delete;
    ~Window() = default;

    Window(Window&&) = default;
    auto operator=(Window &&) -> Window& = default;

    Window(Window const&) = delete;
    auto
    operator=(Window const&) -> Window& = delete;

    auto
    nextEvent() -> std::optional<Event>;

    auto
    update() -> bool;

    auto
    handleMomentaryAction(MomentaryAction const& action) -> void;

    auto
    paused() -> bool;

    auto
    size() -> glm::ivec2
    {
        return m_size;
    }

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
    togglePause();

    void
    close();

    void
    screenshot();

    void
    registerEvent(Event&& event);

    void
    setCallbacks();

    static void
    cursorPositionCB(GLFWwindow* window, double x, double y);

    static void
    keyboardCB(
            GLFWwindow* window,
            int key,
            int scancode,
            int action,
            int mods);

    static void
    mouseButtonCB(GLFWwindow* window, int button, int action, int mods);

    static void
    resizeCB(GLFWwindow* glfwWindow, int width, int height);

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;

    bool m_paused = false;
    glm::ivec2 m_size;
};

#endif
