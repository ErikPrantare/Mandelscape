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

#ifndef MANDELSCAPE_LUA_TESTS_HPP
#define MANDELSCAPE_LUA_TESTS_HPP

#include "lua.hpp"

namespace LuaTests {

TEST_CASE("Lua works", "[lua]")
{
    lua_State* L = luaL_newstate();

    std::string statement = "a = 2*10";
    luaL_dostring(L, statement.c_str());
    lua_getglobal(L, "a");

    REQUIRE(lua_tonumber(L, -1) == 20);
}

}    // namespace LuaTests

#endif
