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

#include "momentaryActionsMap.hpp"

#include "util.hpp"

auto
MomentaryActionsMap::add(KeyDown key, Trigger action) -> void
{
    m_keyMap[key].insert({action});
}

auto
MomentaryActionsMap::operator()(Event const& event) const
        -> std::set<MomentaryAction>
{
    return std::visit([this](auto x) { return this->getActions(x); }, event);
}

auto
MomentaryActionsMap::getActions(KeyDown key) const -> std::set<MomentaryAction>
{
    if(util::contains(m_keyMap, key)) {
        return m_keyMap.at(key);
    }
    return {};
}

auto
MomentaryActionsMap::getActions(MouseMove mouse) -> std::set<MomentaryAction>
{
    return {MouseDelta{mouse.dx, mouse.dy}};
}

auto
MomentaryActionsMap::getActions(KeyUp /*key*/) -> std::set<MomentaryAction>
{
    return {};
}

auto
MomentaryActionsMap::getActions(MouseButtonUp /*mouse*/)
        -> std::set<MomentaryAction>
{
    return {};
}

auto
MomentaryActionsMap::getActions(MouseButtonDown /*mouse*/)
        -> std::set<MomentaryAction>
{
    return {};
}
