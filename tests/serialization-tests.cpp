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
#include "persistentActionMap.hpp"
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

        player.position     = {0.001, 127378888.01, -3.14};
        player.offset       = {0.024, 378888.01, 1e100};
        player.lookAtOffset = {378.99, 1e-10};
        player.scale        = 1.0 / 32897.879423;

        auto ss = std::stringstream();
        serialize(ss, player, "player");

        lua_State* L = luaL_newstate();
        luaL_dostring(L, ss.str().c_str());
        lua_getglobal(L, "player");
        REQUIRE(util::lua::to<Player>(L, -1) == PlayerApprox{player});
        lua_close(L);
    }

    SECTION("X = UniformController")
    {
        auto persistentMap       = PersistentActionMap();
        auto momentaryActionsMap = MomentaryActionsMap();

        persistentMap.add(
                {Input::Key::Key1},
                PersistentAction::ChangeFrequency);
        persistentMap.add(
                {Input::Key::Key1},
                PersistentAction::ChangeTotalOffset);
        persistentMap.add({Input::Key::Key1}, PersistentAction::IncreaseParam);

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

        lua_State* L = luaL_newstate();
        luaL_dostring(L, ss.str().c_str());
        lua_getglobal(L, "uniformController");
        REQUIRE(util::lua::to<UniformController>(L, -1)
                == UniformControllerApprox{uniformController});
        lua_close(L);
    }
}
