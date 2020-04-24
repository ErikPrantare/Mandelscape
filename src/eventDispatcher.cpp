#include "eventDispatcher.h"
#include "utils.h"

void
EventDispatcher::registerKeyDown(
        std::function<void(KeyDown const&)> const callback)
{
    m_keyDownCallbacks.push_back(callback);
}

void
EventDispatcher::dispatch(Event const& event)
{
    std::visit(
            util::overload{[this](KeyDown const& key) {
                               for(auto const& keyDownCallback :
                                   m_keyDownCallbacks) {
                                   keyDownCallback(key);
                               }
                           },
                           [](KeyUp const& key) {},
                           [](MouseMove const& movement) {},
                           [](MouseButtonDown const& button) {},
                           [](MouseButtonUp const& button) {
                           }},
            event);
}
