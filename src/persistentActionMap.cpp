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

#include <algorithm>

#include "util.hpp"

auto
PersistentActionMap::add(KeyDown key, PersistentAction action) -> void
{
    m_inputTriggers[action].insert(Inputs{key});
}

auto
PersistentActionMap::add(MouseButtonDown button, PersistentAction action)
        -> void
{
    m_inputTriggers[action].insert(Inputs{button});
}

auto
PersistentActionMap::updateState(Event const& event) -> void
{
    std::visit(
            util::Overload{
                    [this](KeyDown key) {
                        m_keysDown.insert(key.code);
                        if(util::isModifier(key.code)) {
                            (int&)m_currentMods |= (int)util::toMod(key.code);
                        }
                    },
                    [this](KeyUp key) {
                        m_keysDown.erase(key.code);
                        if(util::isModifier(key.code)) {
                            (int&)m_currentMods &= ~(int)util::toMod(key.code);
                        }
                    },

                    [this](MouseButtonDown button) {
                        m_buttonsDown.insert(button.code);
                    },
                    [this](MouseButtonUp button) {
                        m_buttonsDown.erase(button.code);
                    },
                    // default
                    util::unaryNop},
            event);
}

auto
PersistentActionMap::operator()(PersistentAction action) const -> bool
{
    if(!util::contains(m_inputTriggers, action)) {
        return false;
    }
    for(auto const& trigger : m_inputTriggers.at(action)) {
        if(std::visit(
                   util::Overload{
                           [this](KeyDown key) {
                               return util::contains(m_keysDown, key.code)
                                      && m_currentMods == key.mods;
                           },
                           [this](MouseButtonDown button) {
                               return util::contains(
                                       m_buttonsDown,
                                       button.code);
                           }},
                   trigger)) {
            return true;
        }
    }

    return false;
}
