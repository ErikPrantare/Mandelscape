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
    add(KeyDown key, PersistentAction action) -> void;

    auto
    add(MouseButtonDown button, PersistentAction action) -> void;

    auto
    updateState(Event const& event) -> void;

    [[nodiscard]] auto
    operator()(PersistentAction action) const -> bool;

private:
    std::map<PersistentAction, bool> m_actionMap;
    std::map<KeyDown, std::set<PersistentAction>> m_keyMap;
    std::map<Input::MouseButton, std::set<PersistentAction>> m_mouseButtonMap;

    std::map<PersistentAction, std::set<Event>> m_triggers;

    std::set<Event> m_eventsDown;

    std::set<Input::Key> m_keysDown;
    std::set<Input::MouseButton> m_buttonsDown;

    using Inputs = std::variant<KeyDown, MouseButtonDown>;
    // FIX BETTER NAME
    std::map<PersistentAction, std::set<Inputs>> m_inputTriggers;
    int m_currentMods = 0;
};

#endif
