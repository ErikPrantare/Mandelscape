/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include "util.hpp"

class Window {
public:
    Window(glm::ivec2 size);

    Window()  = delete;
    ~Window() = default;

    Window(Window&&) = default;
    auto
    operator=(Window&&) -> Window& = default;

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

    auto
    pause() noexcept -> void;

    auto
    unpause() noexcept -> void;

    [[nodiscard]] auto
    suspend() noexcept -> util::ScopeGuard;

private:
    auto
    setPaused(bool paused) noexcept -> void;

    // CPP20 decltype(lambda)
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

    glm::dvec2 m_lastMouse = {0.0, 0.0};

    bool m_paused = false;
    glm::ivec2 m_size;
};

#endif
