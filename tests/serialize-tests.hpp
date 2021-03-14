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

#ifndef MANDELLANDSCAPE_SERIALIZE_TESTS_HPP
#define MANDELLANDSCAPE_SERIALIZE_TESTS_HPP

#include "player.hpp"

#include "testUtils.hpp"

namespace SerializeTests {

TEST_CASE("deserialize . serialize == id", "[serialize, deserialize, player]")
{
    auto player = Player();

    player.position     = {0.001, 127378888.01, -3.14};
    player.offset       = {0.024, 378888.01, 1e100};
    player.lookAtOffset = {378.99, 1e-10};
    player.scale        = 1.0 / 32897.879423;

    REQUIRE(deserialize(serialize(player)) == PlayerApprox{player});
}

}    // namespace SerializeTests

#endif
