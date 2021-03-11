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

#ifndef MANDELLANDSCAPE_TRIGGERACTIONSMAP_HPP
#define MANDELLANDSCAPE_TRIGGERACTIONSMAP_HPP

#include <map>
#include <set>
#include <variant>

#include "event.hpp"
#include "momentaryAction.hpp"
#include "glfwEnums.hpp"

class MomentaryActionsMap {
public:
    auto
    add(Input::Key key, TriggerAction action) -> void;

    [[nodiscard]] auto
    operator()(Event const& event) const -> std::set<MomentaryAction>;

private:
    [[nodiscard]] auto
    getActions(KeyDown key) const -> std::set<MomentaryAction>;

    [[nodiscard]] static auto
    getActions(MouseMove mouse) -> std::set<MomentaryAction>;

    [[nodiscard]] static auto
    getActions(KeyUp key) -> std::set<MomentaryAction>;

    [[nodiscard]] static auto
    getActions(MouseButtonUp mouse) -> std::set<MomentaryAction>;

    [[nodiscard]] static auto
    getActions(MouseButtonDown mouse) -> std::set<MomentaryAction>;

    std::map<Input::Key, std::set<MomentaryAction>> m_keyMap;
};

#endif
