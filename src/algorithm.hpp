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

#ifndef MANDELSCAPE_ALGORITHM_HPP
#define MANDELSCAPE_ALGORITHM_HPP

#include <complex>
#include <functional>
#include <memory>

#include <glm/vec2.hpp>

#include "lua.hpp"

namespace algorithm {

struct PointData {
    double height;
    double value;
    bool inside;
};

auto constexpr
operator==(PointData const& a, PointData const& b)
{
    return std::tie(a.height, a.value, a.inside)
           == std::tie(b.height, b.value, b.inside);
}

using Signature = PointData(glm::dvec2 const&, int iterations);

[[nodiscard]] auto
mandelbrot(glm::dvec2 const& pos, int iterations) noexcept -> PointData;

class LuaFunction {
public:
    auto
    operator()(glm::dvec2 const& pos, int iterations) noexcept(false)
            -> PointData;

    friend auto
    fromLua(std::string const& code) noexcept(false)
            -> std::function<Signature>;

private:
    LuaFunction(std::string const& code) noexcept(false);

    struct StateDestructor {
        auto
        operator()(lua_State* state) -> void
        {
            lua_close(state);
        }
    };

    std::unique_ptr<lua_State, StateDestructor> m_state;
};

[[nodiscard]] auto
fromLua(std::string const& code) noexcept(false) -> std::function<Signature>;

}    // namespace algorithm

#endif
