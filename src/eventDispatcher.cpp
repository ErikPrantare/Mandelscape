#include "eventDispatcher.h"
#include "utils.h"

void
EventDispatcher::registerKeyDown(
        std::function<void(KeyDown const&)> const callback)
{
    m_keyDownCallbacks.push_back(callback);
}

void
EventDispatcher::registerKeyUp(std::function<void(KeyUp const&)> const callback)
{
    m_keyUpCallbacks.push_back(callback);
}

void
EventDispatcher::registerMouseMove(
        std::function<void(MouseMove const&)> const callback)
{
    m_mouseMoveCallbacks.push_back(callback);
}

void
EventDispatcher::registerMouseButtonDown(
        std::function<void(MouseButtonDown const&)> const callback)
{
    m_mouseButtonDownCallbacks.push_back(callback);
}

void
EventDispatcher::dispatch(Event const& event)
{
    std::visit(
            util::overload{
                    [this](KeyDown const& key) {
                        for(auto const& keyDownCallback : m_keyDownCallbacks) {
                            keyDownCallback(key);
                        }
                    },
                    [this](KeyUp const& key) {
                        for(auto const& keyUpCallback : m_keyUpCallbacks) {
                            keyUpCallback(key);
                        }
                    },
                    [this](MouseMove const& movement) {
                        for(auto const& mouseMoveCallback :
                            m_mouseMoveCallbacks) {
                            mouseMoveCallback(movement);
                        }
                    },
                    [this](MouseButtonDown const& button) {
                        for(auto const& mouseButtonDownCallback :
                            m_mouseButtonDownCallbacks) {
                            mouseButtonDownCallback(button);
                        }
                    },
                    [this](MouseButtonUp const& button) {
                    }},
            event);
}
