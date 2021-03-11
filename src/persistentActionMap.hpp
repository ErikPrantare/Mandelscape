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

#ifndef MANDELLANDSCAPE_PERSISTENTACTIONMAP_HPP
#define MANDELLANDSCAPE_PERSISTENTACTIONMAP_HPP

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "event.hpp"
#include "persistentAction.hpp"

class PersistentActionMap {
public:
    auto
    add(Input::Key key, PersistentAction action) -> void;

    auto
    add(Input::MouseButton button, PersistentAction action) -> void;

    auto
    updateState(Event const& event) -> void;

    auto
    operator()(PersistentAction action) const -> bool;

private:
    std::map<PersistentAction, bool> m_actionMap;
    std::map<Input::Key, std::set<PersistentAction>> m_keyMap;
    std::map<Input::MouseButton, std::set<PersistentAction>> m_mouseButtonMap;
};

#endif
