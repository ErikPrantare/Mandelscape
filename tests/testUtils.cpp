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

#include "testUtils.hpp"

#include <iostream>

#include <catch2/catch.hpp>
#include <glm/glm.hpp>

#include "player.hpp"
#include "uniformController.hpp"

auto
operator==(double a, Approximate<double> b) -> bool
{
    return a == Approx(b.val);
}

auto
operator==(glm::dvec2 a, Approximate<glm::dvec3> b) -> bool
{
    return a.x == Approximate{b.val.x} && a.y == Approximate{b.val.y};
}

auto
operator==(glm::dvec3 a, Approximate<glm::dvec3> b) -> bool
{
    return a.x == Approximate{b.val.x} && a.y == Approximate{b.val.y}
           && a.z == Approximate{b.val.z};
}

auto
operator==(glm::dvec4 a, Approximate<glm::dvec4> b) -> bool
{
    return a.x == Approximate{b.val.x} && a.y == Approximate{b.val.y}
           && a.z == Approximate{b.val.z} && a.w == Approximate{b.val.w};
}

auto
operator==(glm::dmat4 a, Approximate<glm::dmat4> b) -> bool
{
    return a[0] == Approximate{b.val[0]} && a[1] == Approximate{b.val[1]}
           && a[2] == Approximate{b.val[2]} && a[3] == Approximate{b.val[3]};
}

namespace glm {
auto
operator<<(std::ostream& os, glm::dvec2 const& v) -> std::ostream&
{
    os << '(' << v.x << ", " << v.y << ')';
    return os;
}
auto
operator<<(std::ostream& os, glm::dvec3 const& v) -> std::ostream&
{
    os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
    return os;
}
}    // namespace glm

auto
operator==(Player const& a, PlayerApprox const& b) -> bool
{
    auto aCamera = a.getCamera({640, 720});
    auto bCamera = b.player.getCamera({640, 720});

    return aCamera.cameraSpace()
           == Approximate<glm::dmat4>{bCamera.cameraSpace()};
}

auto
operator==(UniformController const& a, UniformControllerApprox const& b)
        -> bool
{
    return a.m_colorOffset == Approximate{b.uc.m_colorOffset}
           && a.m_colorFrequency == Approx{b.uc.m_colorFrequency}
           && a.m_yScale == Approx{b.uc.m_yScale}
           && a.m_fastMode == b.uc.m_fastMode
           && a.m_iterations == b.uc.m_iterations;
}
