#include "eventDispatcher.h"
#include "utils.h"

void
EventDispatcher::dispatch(Event const& event)
{
    for(auto const& callback : m_callbacks) {
        callback(event);
    }
}
