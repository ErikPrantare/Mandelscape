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

#include "serialization.hpp"

#include "lua.hpp"

#include "player.hpp"
#include "testUtils.hpp"
#include "util.hpp"
#include "stateMap.hpp"
#include "uniformController.hpp"
#include "glfwEnums.hpp"
#include "momentaryActionsMap.hpp"

TEST_CASE(
        "util::lua::toX . serialize == id",
        "[serialize, Player, UniformController]")
{
    SECTION("X = Player")
    {
        auto player = Player();

        player.state().position     = {0.001, 127378888.01, -3.14};
        player.state().offset       = {0.024, 378888.01, 1e100};
        player.state().lookAtOffset = {378.99, 1e-10};
        player.state().scale        = 1.0 / 32897.879423;

        auto ss = std::stringstream();
        serialize(ss, player, "player");

        lua_State* l = luaL_newstate();
        luaL_dostring(l, ss.str().c_str());
        lua_getglobal(l, "player");
        REQUIRE(util::lua::to<Player>(l, -1) == PlayerApprox{player});
        lua_close(l);
    }

    SECTION("X = UniformController")
    {
        auto persistentMap       = StateMap();
        auto momentaryActionsMap = MomentaryActionsMap();

        persistentMap.add({Input::Key::Key1}, State::ChangingFrequency);
        persistentMap.add({Input::Key::Key1}, State::ChangingTotalOffset);
        persistentMap.add({Input::Key::Key1}, State::IncreasingParameter);

        momentaryActionsMap.add(
                {Input::Key::Key1},
                Trigger::IncreaseIterations);
        momentaryActionsMap.add({Input::Key::Key1}, Trigger::ToggleFastMode);

        persistentMap.updateState({KeyDown{Input::Key::Key1}});

        auto uniformController = UniformController();
        uniformController.updateState(persistentMap, 0.317727);

        for(auto const& action :
            momentaryActionsMap({KeyDown{Input::Key::Key1}})) {
            uniformController.handleMomentaryAction(action);
        }

        auto ss = std::stringstream();
        serialize(ss, uniformController, "uniformController");

        lua_State* l = luaL_newstate();
        luaL_dostring(l, ss.str().c_str());
        lua_getglobal(l, "uniformController");
        REQUIRE(util::lua::to<UniformController>(l, -1)
                == UniformControllerApprox{uniformController});
        lua_close(l);
    }
}
