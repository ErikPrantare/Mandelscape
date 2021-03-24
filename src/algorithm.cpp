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

#include "algorithm.hpp"

namespace algorithm {

auto
mandelbrot(glm::dvec2 const& pos, int iterations) noexcept -> PointData
{
    auto c  = std::complex<double>(pos.x, pos.y);
    auto z  = std::complex<double>(0.0, 0.0);
    auto dz = std::complex<double>(0.0, 0.0);

    // main cardioid check
    auto const q = pow(c.real() - 0.25, 2.0) + c.imag() * c.imag();
    if(q * (q + (c.real() - 0.25)) < 0.25 * c.imag() * c.imag()) {
        return {0.0, -1.0, true};
    }

    // period-2 bulb check
    if((c.real() + 1.0) * (c.real() + 1.0) + c.imag() * c.imag()
       < 0.25 * 0.25) {
        return {0.0, -1.0, true};
    }

    for(int i = 0; i < iterations; ++i) {
        dz = 2.0 * z * dz + 1.0;
        z  = z * z + c;

        auto dist = std::abs(z);
        if(dist > 256) {
            auto const r                  = dist;
            auto const dr                 = std::abs(dz);
            auto const distanceEstimation = 2.0 * r * std::log(r) / dr;

            auto constexpr log2 = [](double x) {
                return std::log(x) / std::log(2.0);
            };
            auto const val = i - log2(log2(dist * dist));

            // CPP20 {.height = distanceEstimation...}
            return {distanceEstimation, val, false};
        }
    }

    return {0.0, -1.0, true};
}

auto
makeAlgorithm(lua_State* L) -> std::function<Signature>
{
    return [L](glm::dvec2 const& pos, int iterations) {
        lua_getglobal(L, "pointData");
        lua_pushnumber(L, pos.x);
        lua_pushnumber(L, pos.y);
        lua_pushnumber(L, iterations);
        if(lua_pcall(L, 3, 1, 0) != 0) {
            throw std::runtime_error(lua_tostring(L, -1));
        }
        auto p = algorithm::PointData();

        lua_getfield(L, -1, "height");
        if(lua_isnumber(L, -1) == 0) {
            throw std::runtime_error(
                    "lua function pointData didn't return a table"
                    " containing a number named \"height\"!");
        }
        p.height = lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "inside");
        if(lua_isboolean(L, -1) != 0) {
            p.inside = static_cast<bool>(lua_toboolean(L, -1));
        }
        lua_pop(L, 1);

        lua_getfield(L, -1, "value");
        if(lua_isnumber(L, -1) != 0) {
            p.value = lua_tonumber(L, -1);
        }
        lua_pop(L, 2);
        return p;
    };
}

}    // namespace algorithm
