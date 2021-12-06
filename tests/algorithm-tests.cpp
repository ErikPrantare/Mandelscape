/* Mandelscape
 * Copyright (C) 2021 Erik Präntare
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

#include "algorithm.hpp"

TEST_CASE("algorithm::fromLua", "[fromLua]")
{
    SECTION("Resulting function gets all table entries")
    {
        auto f = algorithm::fromLua(
                "function pointData (re, im, iters)"
                "  return {height = re, value = im, inside = iters == 420}"
                "end");

        // CPP20: {.x = ...}
        REQUIRE(f({12, 13}, 420) == algorithm::PointData{12, 13, true});
    }

    SECTION("Not providing optional fields is ok")
    {
        auto f = algorithm::fromLua(
                "function pointData (re, im, iters)"
                "  return {height = re}"
                "end");

        // CPP20: {.x = ...}
        REQUIRE(f({12, 13}, 420).height == 12);
    }

    SECTION("Not providing mandatory fields throws")
    {
        auto f = algorithm::fromLua(
                "function pointData (re, im, iters)"
                "  return {value = re}"
                "end");

        // CPP20: {.x = ...}
        REQUIRE_THROWS(f({12, 13}, 420));
    }
}
