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

#ifndef MANDELSCAPE_PLAYER_HELPER_TESTS_HPP
#define MANDELSCAPE_PLAYER_HELPER_TESTS_HPP

#include <catch2/catch.hpp>

#include "player.hpp"
#include "playerHelper.hpp"

namespace PlayerHelperTests {

TEST_CASE("Updating offset retains true position", "[PlayerHelper]")
{
    // C++20 {.offset = ...}
    auto player     = Player();
    player.position = {83.0, -1118888.0, 1e-10};
    player.offset   = {3.0, -1e20, -1e-10};

    auto oldPlayer = player;

    PlayerHelper(player).updateOffset({0.888, 123, 2});
    REQUIRE(PlayerHelper(oldPlayer).truePosition()
            == Dvec3Approx{PlayerHelper(player).truePosition()});
}

}    // namespace PlayerHelperTests

#endif
