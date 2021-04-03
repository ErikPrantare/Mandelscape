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
operator==(glm::dvec2 a, Dvec2Approx b) -> bool
{
    return a.x == Approx(b.val.x) && a.y == Approx(b.val.y);
}

auto
operator==(glm::dvec3 a, Dvec3Approx b) -> bool
{
    return a.x == Approx(b.val.x) && a.y == Approx(b.val.y)
           && a.z == Approx(b.val.z);
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
operator<<(std::ostream& os, Player const& player) -> std::ostream&
{
    os << "Player(" << player.position << ", " << player.offset << ", "
       << player.lookAtOffset << ", " << player.scale << ")";
    return os;
}

auto
operator==(Player const& a, PlayerApprox const& b) -> bool
{
    return a.position == Dvec3Approx{b.player.position}
           && a.offset == Dvec3Approx{b.player.offset}
           && a.lookAtOffset == Dvec2Approx{b.player.lookAtOffset}
           && a.scale == Approx{b.player.scale};
}

auto
operator==(UniformController const& a, UniformControllerApprox const& b)
        -> bool
{
    return a.m_colorOffset == Dvec3Approx{b.uc.m_colorOffset}
           && a.m_colorFrequency == Approx{b.uc.m_colorFrequency}
           && a.m_yScale == Approx{b.uc.m_yScale}
           && a.m_fastMode == b.uc.m_fastMode
           && a.m_iterations == b.uc.m_iterations;
}
