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

#include "autoController.hpp"

TEST_CASE("AutoController moves player", "[AutoController]")
{
    auto heightFunc = [](auto x) {
        return x.x;
    };
    auto player     = Player();
    auto controller = AutoController(heightFunc);

    for(auto i = 0; i < 10; ++i) {
        auto prevPos = player.state().position;
        controller.update(player.state(), 1.0);
        REQUIRE(player.state().position != prevPos);
    }
}
