/* Mandelscape
 * Copyright (C) 2021 Erik Präntare
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

#include "serialization.hpp"

template<>
auto
serialize<double>(
        std::ostream& out,
        double const& object,
        std::string const& name,
        int depth) -> void
{
    auto const indentation = std::string(depth, ' ');
    out << indentation << name << " = " << object;
}

template<>
auto
serialize<bool>(
        std::ostream& out,
        bool const& object,
        std::string const& name,
        int depth) -> void
{
    auto const indentation = std::string(depth, ' ');
    out << indentation << name << " = " << (object ? "true" : "false");
}

template<>
auto
serialize<int>(
        std::ostream& out,
        int const& object,
        std::string const& name,
        int depth) -> void
{
    auto const indentation = std::string(depth, ' ');
    out << indentation << name << " = " << object;
}

template<>
auto
deserialize<double>(lua_State* const l, int const offset) -> double
{
    return lua_tonumber(l, offset);
}

template<>
auto
deserialize<bool>(lua_State* const l, int const offset) -> bool
{
    return static_cast<bool>(lua_toboolean(l, offset));
}

template<>
auto
deserialize<int>(lua_State* const l, int const offset) -> int
{
    return static_cast<int>(lua_tonumber(l, offset));
}
