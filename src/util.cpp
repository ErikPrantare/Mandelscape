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
            Key::LeftShift,
            Key::LeftControl,
            Key::LeftAlt,
            Key::LeftSuper,
            Key::RightShift,
            Key::RightControl,
            Key::RightAlt,
            Key::RightSuper,
    };

    return util::contains(modKeys, key);
}

auto
toMod(Input::Key key) noexcept(false) -> Input::Mod
{
    using namespace Input;

    static auto const modKeyMap = std::map<Input::Key, Input::Mod>{
            {Key::LeftShift, Mod::Shift},
            {Key::LeftControl, Mod::Control},
            {Key::LeftAlt, Mod::Alt},
            {Key::LeftSuper, Mod::Super},
            {Key::RightShift, Mod::Shift},
            {Key::RightControl, Mod::Control},
            {Key::RightAlt, Mod::Alt},
            {Key::RightSuper, Mod::Super},
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
    uniformController.m_fastMode = static_cast<bool>(lua_toboolean(L, -1));
    lua_pop(L, 1);

    lua_getfield(L, offset, "iterations");
    uniformController.m_iterations = static_cast<int>(lua_tonumber(L, -1));
    lua_pop(L, 1);

    return uniformController;
}

}    // namespace util::lua

namespace util::nfd {
auto
toNfdFilterItems(std::vector<FilterItem> const& items)
        -> std::vector<nfdnfilteritem_t>
{
    std::vector<nfdnfilteritem_t> nfdFilterItems;
    nfdFilterItems.reserve(items.size());

    for(auto const& item : items) {
        nfdFilterItems.push_back(
                {item.niceName.c_str(), item.suffixes.c_str()});
    }

    return nfdFilterItems;
}

auto
saveDialog(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath,
        string const& defaultName) -> std::pair<fs::path, nfdresult_t>
{
    auto nfdOutput            = NFD::UniquePathN();
    auto const nfdFilterItems = toNfdFilterItems(filterItems);

    auto const result = NFD::SaveDialog(
            nfdOutput,
            nfdFilterItems.data(),
            nfdFilterItems.size(),
            startPath.c_str(),
            defaultName.c_str());

    return {fs::path(nfdOutput.get()), result};
}

auto
openDialog(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath) -> std::pair<fs::path, nfdresult_t>
{
    auto nfdOutput            = NFD::UniquePathN();
    auto const nfdFilterItems = toNfdFilterItems(filterItems);

    auto const result = NFD::OpenDialog(
            nfdOutput,
            nfdFilterItems.data(),
            nfdFilterItems.size(),
            startPath.c_str());

    return {fs::path(nfdOutput.get()), result};
}

auto
openDialogMultiple(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath)
        -> std::pair<std::vector<fs::path>, nfdresult_t>
{
    auto nfdOutput            = NFD::UniquePathSet();
    auto const nfdFilterItems = toNfdFilterItems(filterItems);

    auto const result = NFD::OpenDialogMultiple(
            nfdOutput,
            nfdFilterItems.data(),
            nfdFilterItems.size(),
            startPath.c_str());

    if(result != NFD_OKAY) {
        return {{}, result};
    }

    nfdpathsetsize_t numPaths{};
    NFD::PathSet::Count(nfdOutput, numPaths);

    std::vector<fs::path> paths;
    paths.reserve(numPaths);

    for(nfdpathsetsize_t i = 0; i < numPaths; ++i) {
        auto path = NFD::UniquePathSetPath();
        NFD::PathSet::GetPath(nfdOutput, i, path);
        paths.emplace_back(path.get());
    }

    return {paths, result};
}
}    // namespace util::nfd
