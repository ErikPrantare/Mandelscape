/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include "util.hpp"
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
        bool switchController =
                std::holds_alternative<Trigger>(action)
                && std::get<Trigger>(action) == Trigger::ToggleAutoWalk;

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
