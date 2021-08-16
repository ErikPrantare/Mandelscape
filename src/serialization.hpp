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

#ifndef MANDELSCAPE_SERIALIZATION_HPP
#define MANDELSCAPE_SERIALIZATION_HPP

#include <ostream>
#include <functional>

#include "lua.hpp"

#include "uniformController.hpp"

// CPP20 use concepts for Getter and Setter
template<class Getter, class Setter>
struct SerializationEntry {
    std::string const name;
    Getter const getter;
    Setter const setter;
};

template<auto... member>
auto
makeMemberEntry(std::string const& name)
{
    auto const getter = [](auto const& object) {
        return (object.*....*member);
    };
    auto const setter = [](auto& object, auto const& val) {
        (object.*....*member) = val;
    };

    // CPP20 {.x = ...}
    return SerializationEntry<decltype(getter), decltype(setter)>{
            name,
            getter,
            setter};
}

// CPP20 concept Serializable
template<class Serializable>
struct SerializationTable {
    auto static entries()
    {
        return Serializable::getSerializationTable();
    }
};

template<>
struct SerializationTable<glm::dvec2> {
    auto static entries()
    {
        return std::make_tuple(
                makeMemberEntry<&glm::dvec2::x>("x"),
                makeMemberEntry<&glm::dvec2::y>("y"));
    }
};

template<>
struct SerializationTable<glm::dvec3> {
    auto static entries()
    {
        return std::make_tuple(
                makeMemberEntry<&glm::dvec3::x>("x"),
                makeMemberEntry<&glm::dvec3::y>("y"),
                makeMemberEntry<&glm::dvec3::z>("z"));
    }
};

// CPP20 concept Serializable
template<class Serializable>
auto
serialize(
        std::ostream& out,
        Serializable const& object,
        std::string const& name,
        int depth = 0) -> void
{
    auto const indentation = std::string(depth, ' ');
    out << indentation << name << " = {\n";

    auto const entries        = SerializationTable<Serializable>::entries();
    auto const serializeEntry = [&out, &object, depth](auto const& entry) {
        serialize(out, entry.getter(object), entry.name, depth + 4);
        out << ",\n";
    };

    std::apply(
            [&serializeEntry](auto... x) { (serializeEntry(x), ...); },
            entries);

    out << indentation << "}";
}

template<>
auto
serialize<double>(
        std::ostream& out,
        double const& object,
        std::string const& name,
        int depth) -> void;

auto
serialize(std::ostream&, UniformController const&, std::string const& name)
        -> void;

#endif
