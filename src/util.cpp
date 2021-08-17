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

#include "player.hpp"
#include "uniformController.hpp"

namespace util {

auto
currentDatetimeString() -> std::string
{
    std::time_t const t = std::time(nullptr);
    std::tm const tm    = *std::localtime(&t);
    std::stringstream buffer;
    buffer << std::put_time(&tm, "%Y_%m_%d-%H_%M_%S");
    return buffer.str();
}

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

auto
toGpuVec(glm::dvec3 const v) -> glm::vec3
{
    return static_cast<glm::vec3>(v);
}

}    // namespace util

namespace util::nfd {
namespace literal {
    auto operator""_nfd(char const* str, size_t size)
            -> std::filesystem::path::string_type
    {
        return {str, str + size};
    }
}    // namespace literal

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

    auto path = result == NFD_OKAY ? fs::path{nfdOutput.get()} : fs::path{};

    return {path, result};
}

auto
openDialogWithErrorCode(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath) -> std::pair<fs::path, nfdresult_t>
{
    auto nfdOutput            = NFD::UniquePathN();
    auto const nfdFilterItems = toNfdFilterItems(filterItems);

    auto const code = NFD::OpenDialog(
            nfdOutput,
            nfdFilterItems.data(),
            nfdFilterItems.size(),
            startPath.c_str());

    auto path = code == NFD_OKAY ? fs::path{nfdOutput.get()} : fs::path{};

    return {path, code};
}

auto
openDialog(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath) -> std::optional<fs::path>
{
    auto const& [path, code] = openDialogWithErrorCode(filterItems, startPath);
    if(code != NFD_OKAY) {
        return std::nullopt;
    }
    return std::make_optional(path);
}

auto
openDialogMultipleWithErrorCode(
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

auto
openDialogMultiple(
        std::vector<FilterItem> const& filterItems,
        fs::path const& startPath) -> std::vector<fs::path>
{
    auto const& [paths, code] =
            openDialogMultipleWithErrorCode(filterItems, startPath);

    return paths;
}
}    // namespace util::nfd
