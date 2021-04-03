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

#include "genericController.hpp"

TEST_CASE("No bound functions, no problem", "[GenericController]")
{
    auto gc = GenericController();
    REQUIRE_NOTHROW(gc.handleMomentaryAction({Trigger::Save}));
}

TEST_CASE(
        "GenericController.handleMomentaryAction",
        "[GenericController, handleMomentaryAction]")
{
    bool called = false;
    auto gc = GenericController().withMomentary([&called](auto const& action) {
        if(action == MomentaryAction{Trigger::Save}) {
            called = true;
        }
    });

    SECTION("handleMomentaryAction calls provided function")
    {
        gc.handleMomentaryAction(MomentaryAction{Trigger::Save});
        REQUIRE(called);
    }

    SECTION("Overwriting works")
    {
        gc = gc.withMomentary([](auto const&) {});
        gc.handleMomentaryAction(MomentaryAction{Trigger::Save});
        REQUIRE(!called);
    }
}
