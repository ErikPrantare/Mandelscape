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

struct DataObject {
    glm::dvec3 m_dvec3;
    glm::dvec2 m_dvec2;
    double m_double;
    int m_int;
    bool m_bool;
    struct {
        int m_int;
    } m_nestled;

    auto static getSerializationTable()
    {
        return std::make_tuple(
                makeMemberEntry<&DataObject::m_dvec3>("dvec3"),
                makeMemberEntry<&DataObject::m_dvec2>("dvec2"),
                makeMemberEntry<&DataObject::m_double>("double"),
                makeMemberEntry<&DataObject::m_int>("int"),
                makeMemberEntry<&DataObject::m_bool>("bool"),
                makeMemberEntry<
                        &DataObject::m_nestled,
                        &decltype(m_nestled)::m_int>("nestledInt"));
    }
};

auto
operator==(DataObject const& a, DataObject const& b) -> bool
{
    return std::tie(
                   a.m_dvec3,
                   a.m_dvec2,
                   a.m_double,
                   a.m_int,
                   a.m_bool,
                   a.m_nestled.m_int)
           == std::tie(
                   b.m_dvec3,
                   b.m_dvec2,
                   b.m_double,
                   b.m_int,
                   b.m_bool,
                   b.m_nestled.m_int);
}

TEST_CASE(
        "deserialize . serialize == id",
        "[serialize, Player, UniformController]")
{
    // CPP20 {.x = ...}
    auto data = DataObject{
            {0.7, -10e7, 837},
            {-10e7, 837},
            13.37,
            13,
            true,
            {420}};

    auto ss = std::stringstream();
    serialize(ss, data, "data");
    lua_State* l = luaL_newstate();
    luaL_dostring(l, ss.str().c_str());
    lua_getglobal(l, "data");
    REQUIRE(deserialize<DataObject>(l, -1) == data);
    lua_close(l);
}
