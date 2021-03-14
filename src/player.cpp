#include "player.hpp"

#include "lua.hpp"

auto
serialize(Player const& player) -> std::string
{
    // C++20 use fmt library instead
    std::string const format = R"(player = {
    position = {x = %.16f, y = %.16f, z = %.16f},
    positionOffset = {x = %.16f, y = %.16f, z = %.16f},
    lookAtOffset = {x = %.16f, y = %.16f},
    scale = %.16f
})";

    auto const write = [&format, &player](char* output, size_t size) {
        return std::snprintf(    // NOLINT
                output,
                size,
                format.c_str(),
                player.position.x,
                player.position.y,
                player.position.z,
                player.positionOffset.x,
                player.positionOffset.y,
                player.positionOffset.z,
                player.lookAtOffset.x,
                player.lookAtOffset.y,
                player.scale);
    };

    auto output = std::string();
    output.resize(write(nullptr, 0));
    write(output.data(), output.size() + 1);

    return output;
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

    lua_getfield(L, -1, "positionOffset");
    player.positionOffset = util::lua::toVec3(L, -1);
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
    return std::tie(a.position, a.positionOffset, a.lookAtOffset, a.scale)
           == std::tie(b.position, b.positionOffset, b.lookAtOffset, b.scale);
}
