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

#ifndef MANDELLANDSCAPE_TOACTIONS_TESTS_HPP
#define MANDELLANDSCAPE_TOACTIONS_TESTS_HPP

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "momentaryActionsMap.hpp"
#include "event.hpp"
#include "util.hpp"

namespace MomentaryActionsMapTests {

TEST_CASE("MomentaryActionsMap maps from glfwEvent", "[MomentaryActionsMap]")
{
    using namespace Input;

    MomentaryActionsMap map;
    map.add(Key::U, TriggerAction::IncreaseIterations);
    map.add(Key::U, TriggerAction::DecreaseIterations);

    SECTION("Mapping gives correct result")
    {
        Event event  = KeyDown{Key::U};
        auto actions = map(event);

        REQUIRE(actions.size() == 2);
        REQUIRE(util::contains(actions, {TriggerAction::IncreaseIterations}));
        REQUIRE(util::contains(actions, {TriggerAction::DecreaseIterations}));
    }

    SECTION("No given mapping -> empty container")
    {
        auto event   = KeyDown{Key::K};
        auto actions = map(event);
        REQUIRE(actions.size() == 0);
    }
}

}    // namespace MomentaryActionsMapTests

#endif
