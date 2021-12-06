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

#include <catch2/catch.hpp>

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "momentaryActionsMap.hpp"
#include "event.hpp"
#include "util.hpp"

TEST_CASE("MomentaryActionsMap maps from glfwEvent", "[MomentaryActionsMap]")
{
    using namespace Input;

    MomentaryActionsMap map;
    map.add({Key::U}, Trigger::IncreaseIterations);
    map.add({Key::U}, Trigger::DecreaseIterations);
    map.add({Key::U, (int)Mod::Shift}, Trigger::Save);

    SECTION("No given mapping -> empty container")
    {
        auto event   = KeyDown{Key::K};
        auto actions = map(event);
        REQUIRE(actions.empty());
    }

    SECTION("Mapping gives correct result")
    {
        Event event  = KeyDown{Key::U};
        auto actions = map(event);

        REQUIRE(actions.size() == 2);
        REQUIRE(util::contains(actions, {Trigger::IncreaseIterations}));
        REQUIRE(util::contains(actions, {Trigger::DecreaseIterations}));
    }

    SECTION("Mapping handles mods")
    {
        Event event  = KeyDown{Key::U, (int)Mod::Shift};
        auto actions = map(event);

        REQUIRE(actions.size() == 1);
        REQUIRE(util::contains(actions, {Trigger::Save}));
    }
}
