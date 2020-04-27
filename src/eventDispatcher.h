#ifndef MANDELLANDSCAPE_EVENTDISPATCHER_H
#define MANDELLANDSCAPE_EVENTDISPATCHER_H

#include <functional>
#include <vector>
#include <utility>
#include <iostream>

#include "event.h"
#include "utils.h"
#include "mandelTypeTraits.h"

class EventDispatcher {
public:
    // TODO: Return object that when destroyed will remove the callback
    template<class T>
    decltype(std::holds_alternative<T>(std::declval<Event>()), void())
    registerCallback(std::function<void(T const&)> callback)
    {
        m_callbacks.push_back([callback](Event const& event) {
            std::visit(
                    util::overload{
                            [](auto v) -> Require<!SpecialisationOf<
                                               decltype(v),
                                               std::variant>::value> {},
                            [callback](T v) {
                                callback(v);
                            }},
                    event);
        });
    }

    void
    dispatch(Event const& event);

private:
    std::vector<std::function<void(Event const&)>> m_callbacks;
};

#endif
