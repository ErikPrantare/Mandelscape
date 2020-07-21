#ifndef MANDELLANDSCAPE_META_CONTROLLER_HPP
#define MANDELLANDSCAPE_META_CONTROLLER_HPP

#include "event.hpp"
#include "playerController.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <variant>
#include <array>

auto constexpr isNextController(Event const& event) -> bool
{
    return std::holds_alternative<KeyDown>(event)
                   ? std::get<KeyDown>(event).code == GLFW_KEY_C
                   : false;
}

auto constexpr safeIncrement(size_t current, size_t bound, size_t stepSize)
        -> size_t
{
    return current + stepSize < bound ? current + stepSize : 0;
}

template<size_t numControllers>
class Metacontroller {
public:
    template<class... Controllers>
    Metacontroller(Controllers&&... controllers) : m_controllers{controllers...}
    {}

    auto
    handleEvent(Event const& event) -> void
    {
        if(isNextController(event)) {
            m_activeController =
                    safeIncrement(m_activeController, numControllers, 1);
        }
        else {
            m_controllers[m_activeController].handleEvent(event);
        }
    }

    auto
    update(Player& player, double deltaSeconds) -> void
    {
        m_controllers[m_activeController].update(player, deltaSeconds);
    }

private:
    std::array<PlayerController, numControllers> m_controllers;
    size_t m_activeController = 0;
};

template<class... Controllers>
Metacontroller(Controllers...) -> Metacontroller<sizeof...(Controllers)>;

#endif    // MANDELLANDSCAPE_META_CONTROLLER_HPP
