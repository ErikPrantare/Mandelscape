#include "player.hpp"

#include <sstream>

#include "lua.hpp"

auto
serialize(Player const& player) -> std::string
{
    // C++20 use fmt library instead. formatting + locales suck
    //    std::string const format = R"(player = {
    //    position = {x = %.16f, y = %.16f, z = %.16f},
    //    offset = {x = %.16f, y = %.16f, z = %.16f},
    //    lookAtOffset = {x = %.16f, y = %.16f},
    //    scale = %.16f
    //})";

    auto ss = std::stringstream();
    ss << "player = {\nposition = {x = " << player.position.x
       << ", y = " << player.position.y << ", z = " << player.position.z
       << "},\noffset = {x = " << player.offset.x
       << ", y = " << player.offset.y << ", z = " << player.offset.z
       << "},\nlookAtOffset = {x = " << player.lookAtOffset.x
       << ", y = " << player.lookAtOffset.y << "},\nscale = " << player.scale
       << "\n}";

    return ss.str();
}

auto
deserialize(std::string const& input) -> Player
{
    lua_State* L = luaL_newstate();
    luaL_dostring(L, input.c_str());

    auto player = Player();

    lua_getglobal(L, "player");

    lua_getfield(L, -1, "position");
    player.position = util::lua::toVec3(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "offset");
    player.offset = util::lua::toVec3(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "lookAtOffset");
    player.lookAtOffset = util::lua::toVec2(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, -1, "scale");
    player.scale = lua_tonumber(L, -1);
    lua_pop(L, 2);

    lua_close(L);

    return player;
}

[[nodiscard]] auto
operator==(Player const& a, Player const& b) -> bool
{
    return std::tie(a.position, a.offset, a.lookAtOffset, a.scale)
           == std::tie(b.position, b.offset, b.lookAtOffset, b.scale);
}
