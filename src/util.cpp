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

#include "util.hpp"

#include <glm/glm.hpp>

namespace util {

auto
unitVec2(double theta) noexcept -> glm::dvec2
{
    return {std::cos(theta), std::sin(theta)};
}

auto
isModifier(Input::Key key) noexcept -> bool
{
    using namespace Input;

    static auto const modKeys = std::set<Input::Key>{
            Key::LEFT_SHIFT,
            Key::LEFT_CONTROL,
            Key::LEFT_ALT,
            Key::LEFT_SUPER,
            Key::RIGHT_SHIFT,
            Key::RIGHT_CONTROL,
            Key::RIGHT_ALT,
            Key::RIGHT_SUPER,
    };

    return util::contains(modKeys, key);
}

auto
toMod(Input::Key key) noexcept(false) -> Input::Mod
{
    using namespace Input;

    static auto const modKeyMap = std::map<Input::Key, Input::Mod>{
            {Key::LEFT_SHIFT, Mod::SHIFT},
            {Key::LEFT_CONTROL, Mod::CONTROL},
            {Key::LEFT_ALT, Mod::ALT},
            {Key::LEFT_SUPER, Mod::SUPER},
            {Key::RIGHT_SHIFT, Mod::SHIFT},
            {Key::RIGHT_CONTROL, Mod::CONTROL},
            {Key::RIGHT_ALT, Mod::ALT},
            {Key::RIGHT_SUPER, Mod::SUPER},
    };

    return modKeyMap.at(key);
}

}    // namespace util

namespace util::lua {

auto
toVec3(lua_State* L, int offset) -> glm::dvec3
{
    auto vec = glm::dvec3();

    lua_getfield(L, offset, "x");
    vec.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, offset, "y");
    vec.y = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, offset, "z");
    vec.z = lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vec;
}

auto
toVec2(lua_State* L, int offset) -> glm::dvec2
{
    auto vec = glm::dvec2();

    lua_getfield(L, offset, "x");
    vec.x = lua_tonumber(L, -1);
    lua_pop(L, 1);
    lua_getfield(L, offset, "y");
    vec.y = lua_tonumber(L, -1);
    lua_pop(L, 1);

    return vec;
}

auto
toPlayer(lua_State* L, int offset) -> Player
{
    auto player = Player();

    lua_getfield(L, offset, "position");
    player.position = util::lua::toVec3(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, offset, "offset");
    player.offset = util::lua::toVec3(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, offset, "lookAtOffset");
    player.lookAtOffset = util::lua::toVec2(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, offset, "scale");
    player.scale = lua_tonumber(L, -1);
    lua_pop(L, 1);

    return player;
}

auto
toUniformController(lua_State* L, int offset) -> UniformController
{
    auto uniformController = UniformController();

    lua_getfield(L, offset, "colorOffset");
    uniformController.m_colorOffset = util::lua::toVec3(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, offset, "colorFrequency");
    uniformController.m_colorFrequency = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, offset, "yScale");
    uniformController.m_yScale = lua_tonumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, offset, "fastMode");
    uniformController.m_fastMode = lua_toboolean(L, -1);
    lua_pop(L, 1);

    return uniformController;
}

}    // namespace util::lua
