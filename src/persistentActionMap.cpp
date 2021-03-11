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

#include "persistentActionMap.hpp"

#include "util.hpp"

auto
PersistentActionMap::add(Input::Key key, PersistentAction action) -> void
{
    m_keyMap[key].insert(action);
}

auto
PersistentActionMap::add(Input::MouseButton button, PersistentAction action)
        -> void
{
    m_mouseButtonMap[button].insert(action);
}

auto
PersistentActionMap::updateState(Event const& event) -> void
{
    auto const setActions = [this](std::set<PersistentAction>& actions,
                                   bool value) {
        for(auto const& action : actions) {
            m_actionMap[action] = value;
        }
    };

    std::visit(
            util::Overload{
                    [this, &setActions](KeyDown key) {
                        setActions(m_keyMap[key.code], true);
                    },
                    [this, &setActions](KeyUp key) {
                        setActions(m_keyMap[key.code], false);
                    },

                    [this, &setActions](MouseButtonDown button) {
                        setActions(m_mouseButtonMap[button.button], true);
                    },
                    [this, &setActions](MouseButtonUp button) {
                        setActions(m_mouseButtonMap[button.button], false);
                    },
                    // default
                    util::unaryNOP},
            event);
}

auto
PersistentActionMap::operator()(PersistentAction action) const -> bool
{
    return util::contains(m_actionMap, action) && m_actionMap.at(action);
}
