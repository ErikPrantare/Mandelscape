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

#include "window.hpp"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <variant>
#include <filesystem>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#include "event.hpp"
#include "util.hpp"

void
glfwErrorCallback(int code, char const* description)
{
    std::cerr << "Error code: " << code << "\nDescription: " << description
              << '\n';
}

auto
createWindow(glm::ivec2 const size) -> GLFWwindow*
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    glfwSetErrorCallback(&glfwErrorCallback);

    return glfwCreateWindow(size.x, size.y, "Mandelscape", nullptr, nullptr);
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
    glEnable(GL_MULTISAMPLE);
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
    glfwSetFramebufferSizeCallback(m_window.get(), &resizeCB);
}

auto
Window::nextEvent() -> std::optional<Event>
{
    return util::pop(m_events);
}

auto
Window::update() -> bool
{
    glfwMakeContextCurrent(m_window.get());
    glfwSwapBuffers(m_window.get());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    return glfwWindowShouldClose(m_window.get()) == 0;
}

auto
Window::handleMomentaryAction(MomentaryAction const& action) -> void
{
    if(std::holds_alternative<Trigger>(action)) {
        switch(std::get<Trigger>(action)) {
        case Trigger::TogglePause:
            togglePause();
            break;
        case Trigger::CloseWindow:
            close();
            break;
        default:
            break;
        }
    }
}

auto
Window::paused() const noexcept -> bool
{
    return m_paused;
}

auto
Window::pause() noexcept -> void
{
    setPaused(true);
}

auto
Window::unpause() noexcept -> void
{
    setPaused(false);
}

auto
Window::setPaused(bool paused) noexcept -> void
{
    m_paused = paused;
    glfwSetInputMode(
            m_window.get(),
            GLFW_CURSOR,
            m_paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

auto
Window::suspend() noexcept -> util::ScopeGuard
{
    auto const resume = [this, previous = m_paused] {
        setPaused(previous);
    };
    pause();
    return {resume};
}

auto
Window::togglePause() noexcept -> void
{
    setPaused(!m_paused);
}

auto
Window::size() const noexcept -> glm::ivec2
{
    return m_size;
}

void
Window::close()
{
    glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
}

void
Window::resizeBuffer(glm::ivec2 const size)
{
    m_size = size;

    glfwMakeContextCurrent(m_window.get());
    glViewport(0, 0, m_size.x, m_size.y);
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
        window->registerEvent(KeyDown{static_cast<Input::Key>(key), mods});
    } break;

    case GLFW_RELEASE: {
        window->registerEvent(KeyUp{static_cast<Input::Key>(key), mods});
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
        window->registerEvent(
                MouseButtonDown{static_cast<Input::MouseButton>(button)});
    } break;
    case GLFW_RELEASE: {
        window->registerEvent(
                MouseButtonUp{static_cast<Input::MouseButton>(button)});
    }
    }
}

void
Window::resizeCB(GLFWwindow* glfwWindow, int width, int height)
{
    auto* window = static_cast<Window*>(glfwGetWindowUserPointer(glfwWindow));

    window->resizeBuffer({width, height});
}
