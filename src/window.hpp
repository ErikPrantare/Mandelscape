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
#include "framebuffer.hpp"

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

    [[nodiscard]] auto
    nextEvent() -> std::optional<Event>;

    [[nodiscard]] auto
    update() -> bool;

    auto
    handleMomentaryAction(MomentaryAction const& action) -> void;

    [[nodiscard]] auto
    paused() const noexcept -> bool;

    [[nodiscard]] auto
    size() const noexcept -> glm::ivec2;

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

    auto
    togglePause() noexcept -> void;

    auto
    close() -> void;

    auto
    screenshot() noexcept(false) -> void;

    auto
    resizeBuffer(glm::ivec2 size) -> void;

    auto
    registerEvent(Event&& event) -> void;

    auto
    setCallbacks() -> void;

    static auto
    cursorPositionCB(GLFWwindow* window, double x, double y) -> void;

    static auto
    keyboardCB(GLFWwindow* window, int key, int scancode, int action, int mods)
            -> void;

    static auto
    mouseButtonCB(GLFWwindow* window, int button, int action, int mods)
            -> void;

    static auto
    resizeCB(GLFWwindow* glfwWindow, int width, int height) -> void;

    double m_lastMouseX = 0.0;
    double m_lastMouseY = 0.0;

    int m_queueScreenshot                         = false;
    bool m_paused                                 = false;
    std::optional<Framebuffer> m_screenshotBuffer = std::nullopt;
    glm::ivec2 m_size;
};

#endif
