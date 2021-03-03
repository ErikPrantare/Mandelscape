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
