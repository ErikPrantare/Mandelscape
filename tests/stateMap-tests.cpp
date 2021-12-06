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
#include "stateMap.hpp"
#include "event.hpp"
#include "util.hpp"

TEST_CASE(
        "PersistentActionMap keeps track of keyboard state",
        "[PersistentActionMap]")
{
    using namespace Input;

    auto const action    = State::MovingForwards;
    auto const modAction = State::MovingBackwards;

    auto const key    = KeyDown{Key::U};
    auto const modKey = KeyDown{Key::U, (int)Mod::Shift};

    auto map = StateMap();
    map.add(key, action);
    map.add(modKey, modAction);
    SECTION("Action defaults to off")
    {
        REQUIRE(!map(action));
    }

    SECTION("State is updated")
    {
        map.updateState(key);
        REQUIRE(map(action));
        REQUIRE(!map(modAction));

        map.updateState(KeyUp{key.code});
        REQUIRE(!map(action));
    }

    SECTION("Modifiers override earlier keypresses")
    {
        map.updateState(key);
        REQUIRE(map(action));

        map.updateState(KeyDown{Key::LeftShift});
        REQUIRE(!map(action));
        REQUIRE(map(modAction));

        map.updateState(KeyUp{Key::LeftShift});
        REQUIRE(map(action));
        REQUIRE(!map(modAction));
    }
}
