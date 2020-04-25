#ifndef MANDELLANDSCAPE_EVENTDISPATCHER_H
#define MANDELLANDSCAPE_EVENTDISPATCHER_H

#include <functional>
#include <vector>

#include "event.h"

class EventDispatcher {
public:
    // TODO: Return object that when destroyed will remove the callback
    void registerKeyDown(std::function<void(KeyDown const&)>);
    void registerKeyUp(std::function<void(KeyUp const&)>);
    void registerMouseMove(std::function<void(MouseMove const&)>);
    void registerMouseButtonDown(std::function<void(MouseButtonDown const&)>);

    void
    dispatch(Event const& event);

private:
    std::vector<std::function<void(KeyDown const&)>> m_keyDownCallbacks;
    std::vector<std::function<void(KeyUp const&)>> m_keyUpCallbacks;
    std::vector<std::function<void(MouseMove const&)>> m_mouseMoveCallbacks;
    std::vector<std::function<void(MouseButtonDown const&)>>
            m_mouseButtonDownCallbacks;
};

#endif
