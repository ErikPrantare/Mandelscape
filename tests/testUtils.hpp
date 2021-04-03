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

#ifndef MANDELLANDSCAPE_TEST_UTILS_HPP
#define MANDELLANDSCAPE_TEST_UTILS_HPP

#include <iostream>

#include <catch2/catch.hpp>
#include <glm/glm.hpp>

#include "player.hpp"
#include "uniformController.hpp"

struct Dvec2Approx {
    glm::dvec2 val;
};

auto
operator==(glm::dvec2 a, Dvec2Approx b) -> bool;

struct Dvec3Approx {
    glm::dvec3 val;
};

auto
operator==(glm::dvec3 a, Dvec3Approx b) -> bool;

// namespace glm needed for ADL
namespace glm {
auto
operator<<(std::ostream& os, glm::dvec2 const& v) -> std::ostream&;
auto
operator<<(std::ostream& os, glm::dvec3 const& v) -> std::ostream&;

}    // namespace glm

auto
operator<<(std::ostream& os, Player const& player) -> std::ostream&;

struct PlayerApprox {
    Player player;
};

auto
operator==(Player const& a, PlayerApprox const& b) -> bool;

struct UniformControllerApprox {
    UniformController uc;
};

auto
operator==(UniformController const& a, UniformControllerApprox const& b)
        -> bool;

#endif
