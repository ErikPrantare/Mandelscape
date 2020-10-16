#ifndef MANDELLANDSCAPE_META_CONTROLLER_HPP
#define MANDELLANDSCAPE_META_CONTROLLER_HPP

#include "event.hpp"
#include "playerController.hpp"
#include "utils.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <type_traits>
#include <variant>
#include <array>
#include <memory>

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
        bool switchController = std::holds_alternative<KeyDown>(event)
                                && std::get<KeyDown>(event).code == GLFW_KEY_C;

        if(switchController) {
            m_activeController = (m_activeController + 1) % numControllers;
        }

        m_controllers[m_activeController]->handleEvent(event);
    }

    auto
    update(Player* const player, double dt) -> void
    {
        m_controllers[m_activeController]->update(player, dt);
    }

private:
    std::array<std::unique_ptr<PlayerController>, numControllers>
            m_controllers;
    size_t m_activeController = 0;
};

template<class... Controllers>
MetaController(Controllers...) -> MetaController<sizeof...(Controllers)>;

#endif    // MANDELLANDSCAPE_META_CONTROLLER_HPP
