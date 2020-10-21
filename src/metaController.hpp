#ifndef MANDELLANDSCAPE_META_CONTROLLER_HPP
#define MANDELLANDSCAPE_META_CONTROLLER_HPP

#include <type_traits>
#include <variant>
#include <array>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "event.hpp"
#include "playerController.hpp"
#include "utils.hpp"
#include "momentaryAction.hpp"

template<size_t numControllers>
class MetaController {
public:
    template<class... Controllers>
    MetaController(Controllers&&... controllers) :
                m_controllers{std::move(controllers)...}
    {}

    auto
    handleMomentaryAction(MomentaryAction const& action) -> void
    {
        bool switchController = std::holds_alternative<TriggerAction>(action)
                                && std::get<TriggerAction>(action)
                                           == TriggerAction::ToggleAutoWalk;

        if(switchController) {
            m_activeController = (m_activeController + 1) % numControllers;
        }

        m_controllers[m_activeController]->handleMomentaryAction(action);
    }

    auto
    updateState(PersistentActionMap const& actionMap) -> void
    {
        m_controllers[m_activeController]->updateState(actionMap);
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
