#ifndef MANDELLANDSCAPE_META_CONTROLLER_HPP
#define MANDELLANDSCAPE_META_CONTROLLER_HPP

#include "event.hpp"
#include "playerController.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <type_traits>
#include <variant>
#include <array>
#include <memory>

auto constexpr isNextController(Event const& event) -> bool
{
    return std::holds_alternative<KeyDown>(event)
                   ? std::get<KeyDown>(event).code == GLFW_KEY_C
                   : false;
}

auto constexpr safeIncrement(size_t current, size_t bound) -> size_t
{
    return ++current < bound ? current : 0Lu;
}

template<size_t numControllers>
class MetaController {
public:
    template<class... Controllers>
    MetaController(Controllers&&... controllers) :
                m_controllers{std::move(controllers)...}
    {}

    auto
    handleEvent(Event const& event) -> void
    {
        if(isNextController(event)) {
            m_activeController =
                    safeIncrement(m_activeController, numControllers);
        }
        else {
            m_controllers[m_activeController]->handleEvent(event);
        }
    }

    auto
    update(Player& player, double deltaSeconds) -> void
    {
        m_controllers[m_activeController]->update(player, deltaSeconds);
    }

private:
    std::array<std::unique_ptr<PlayerController>, numControllers> m_controllers;
    size_t m_activeController = 0;
};

template<class... Controllers>
MetaController(Controllers...) -> MetaController<sizeof...(Controllers)>;

#endif    // MANDELLANDSCAPE_META_CONTROLLER_HPP
