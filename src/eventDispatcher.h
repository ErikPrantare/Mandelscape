#ifndef MANDELLANDSCAPE_EVENTDISPATCHER_H
#define MANDELLANDSCAPE_EVENTDISPATCHER_H

#include <functional>
#include <vector>

#include "event.h"

class EventDispatcher {
public:
    // TODO: Return object that when destroyed will remove the callback
    void registerKeyDown(std::function<void(KeyDown const&)>);

    void
    dispatch(Event const& event);

private:
    std::vector<std::function<void(KeyDown const&)>> m_keyDownCallbacks;
};

#endif
