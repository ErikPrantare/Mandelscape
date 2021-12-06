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

#include <queue>
#include <optional>

#include "util.hpp"

TEST_CASE(
        "util::pop pops and returns the top element of a std::queue",
        "[utils]")
{
    auto queue = std::queue<int>();

    SECTION("Empty queue should return a std::nullopt")
    {
        auto const nullopt = util::pop(queue);
        REQUIRE(!nullopt.has_value());
    }
}
