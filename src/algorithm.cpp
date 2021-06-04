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
        return {0.0, static_cast<double>(iterations), true};
    }

    // period-2 bulb check
    if((c.real() + 1.0) * (c.real() + 1.0) + c.imag() * c.imag()
       < 0.25 * 0.25) {
        return {0.0, static_cast<double>(iterations), true};
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

    return {0.0, static_cast<double>(iterations), true};
}

LuaFunction::LuaFunction(std::string const& code) noexcept(false) :
            m_state(luaL_newstate())
{
    auto* const l = m_state.get();
    luaopen_math(l);
    if(luaL_dostring(l, code.c_str()) != 0) {
        throw std::runtime_error(lua_tostring(l, -1));
    }
}

auto
LuaFunction::operator()(glm::dvec2 const& pos, int iterations) noexcept(false)
        -> PointData
{
    auto* const l = m_state.get();

    lua_getglobal(l, "pointData");
    lua_pushnumber(l, pos.x);
    lua_pushnumber(l, pos.y);
    lua_pushnumber(l, iterations);
    if(lua_pcall(l, 3, 1, 0) != 0) {
        throw std::runtime_error(lua_tostring(l, -1));
    }
    auto p = algorithm::PointData();

    lua_getfield(l, -1, "height");
    if(lua_isnumber(l, -1) == 0) {
        throw std::runtime_error(
                "lua function pointData didn't return a table"
                " containing a number named \"height\"!");
    }
    p.height = lua_tonumber(l, -1);
    lua_pop(l, 1);

    lua_getfield(l, -1, "inside");
    if(lua_isboolean(l, -1) != 0) {
        p.inside = static_cast<bool>(lua_toboolean(l, -1));
    }
    lua_pop(l, 1);

    lua_getfield(l, -1, "value");
    if(lua_isnumber(l, -1) != 0) {
        p.value = lua_tonumber(l, -1);
    }
    lua_pop(l, 2);
    return p;
}

auto
fromLua(std::string const& code) noexcept(false) -> std::function<Signature>
{
    // ugly hack, but std::function must be copyable which LuaFunction isn't
    auto f = std::make_shared<LuaFunction>(LuaFunction(code));
    return [f = std::move(f)](glm::dvec2 const& pos, int iterations) {
        return (*f)(pos, iterations);
    };
}

}    // namespace algorithm
