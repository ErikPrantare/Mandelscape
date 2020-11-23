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
    glfwSetWindowSizeCallback(m_window.get(), &resizeCB);
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
    if(m_screenshotBuffer != std::nullopt) {
        screenshot();
        m_screenshotBuffer->unbind();
        m_screenshotBuffer = std::nullopt;
        glViewport(0, 0, m_size.x, m_size.y);
    }
    glfwSwapBuffers(m_window.get());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    return glfwWindowShouldClose(m_window.get()) == 0;
}

auto
Window::handleMomentaryAction(MomentaryAction const& action) -> void
{
    if(std::holds_alternative<TriggerAction>(action)) {
        switch(std::get<TriggerAction>(action)) {
        case TriggerAction::TogglePause:
            togglePause();
            break;
        case TriggerAction::TakeScreenshot:
            m_screenshotBuffer = Framebuffer(2 * m_size);
            m_screenshotBuffer->bind();
            glViewport(0, 0, 2 * m_size.x, 2 * m_size.y);
            break;
        case TriggerAction::CloseWindow:
            close();
            break;
        default:
            break;
        }
    }
}

auto
Window::togglePause() noexcept -> void
{
    m_paused = !m_paused;
    glfwSetInputMode(
            m_window.get(),
            GLFW_CURSOR,
            m_paused ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

auto
Window::paused() const noexcept -> bool
{
    return m_paused;
}

auto
Window::size() const noexcept -> glm::ivec2
{
    return m_screenshotBuffer ? m_screenshotBuffer->size() : m_size;
}

void
Window::close()
{
    glfwSetWindowShouldClose(m_window.get(), GLFW_TRUE);
}

static auto
saveScreenshot(std::vector<unsigned char>& pixels, glm::ivec2 size) -> void
{
    std::time_t const t = std::time(nullptr);
    std::tm const tm    = *std::localtime(&t);
    std::stringstream buffer;
    buffer << std::put_time(&tm, "%Y_%m_%d-%H_%M_%S");

    namespace fs          = std::filesystem;
    std::string const dir = "screenshots";
    if(!fs::is_directory(dir) || !fs::exists(dir)) {
        fs::create_directory(dir);
    }

    std::string filename = dir + "/" + buffer.str() + ".png";

    stbi_flip_vertically_on_write(1);
    stbi_write_png(filename.c_str(), size.x, size.y, 3, pixels.data(), 0);
}

void
Window::screenshot()
{
    glfwMakeContextCurrent(m_window.get());

    auto const inputSize  = m_screenshotBuffer->size();
    auto const outputSize = inputSize / 2;
    auto const pixels     = m_screenshotBuffer->readPixels();

    std::vector<unsigned char> aa(3 * outputSize.x * outputSize.y);

    stbir_resize_uint8(
            pixels.data(),
            inputSize.x,
            inputSize.y,
            0,
            aa.data(),
            outputSize.x,
            outputSize.y,
            0,
            3);

    saveScreenshot(aa, outputSize);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
