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

#ifndef MANDELLANDSCAPE_PERSISTENTACTIONMAP_TESTS_HPP
#define MANDELLANDSCAPE_PERSISTENTACTIONMAP_TESTS_HPP

#include <map>
#include <set>

#include "glfwEnums.hpp"
#include "persistentActionMap.hpp"
#include "event.hpp"
#include "util.hpp"

namespace PersistentActionMapTests {

TEST_CASE(
        "PersistentActionMap keeps track of keyboard state",
        "[PersistentActionMap]")
{
    using namespace Input;

    PersistentActionMap map;
    map.add(Key::U, PersistentAction::MoveForwards);
    SECTION("State is updated")
    {
        map.updateState(KeyDown{Key::U});
        REQUIRE(map(PersistentAction::MoveForwards));

        map.updateState(KeyUp{Key::U});
        REQUIRE(!map(PersistentAction::MoveForwards));
    }

    SECTION("Action defaults to off")
    {
        REQUIRE(!map(PersistentAction::MoveBackwards));
    }
}

}    // namespace PersistentActionMapTests

#endif
