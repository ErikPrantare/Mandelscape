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

#ifndef MANDELSCAPE_UNIFORM_CONTROLLER_HPP
#define MANDELSCAPE_UNIFORM_CONTROLLER_HPP

#include <glm/glm.hpp>

#include "lua.hpp"

#include "stateMap.hpp"
#include "shaderProgram.hpp"
#include "momentaryAction.hpp"
#include "util.hpp"
#include "serialization.hpp"

class UniformController;
// used in tests to test for approximate equality
class UniformControllerApprox;
auto
operator==(UniformController const& a, UniformControllerApprox const& b)
        -> bool;

/* Controls shader parameters */
class UniformController {
public:
    auto
    updateState(StateMap const& active, double dt) noexcept -> void;

    auto
    handleMomentaryAction(MomentaryAction const& action) -> void;

    auto
    update(ShaderProgram& shaderProgram) -> void;

    [[nodiscard]] auto
    yScale() const noexcept -> double;

    [[nodiscard]] auto
    iterations() const noexcept -> int;

    [[nodiscard]] auto
    operator==(UniformController const& other) const noexcept -> bool;

    [[nodiscard]] auto static getSerializationTable()
    {
        return std::make_tuple(
                makeMemberEntry<&UniformController::m_colorOffset>(
                        "colorOffset"),
                makeMemberEntry<&UniformController::m_colorFrequency>(
                        "colorFrequency"),
                makeMemberEntry<&UniformController::m_yScale>("yScale"),
                makeMemberEntry<&UniformController::m_fastMode>("fastMode"),
                makeMemberEntry<&UniformController::m_iterations>(
                        "iterations"));
    }

    friend auto
    operator==(UniformController const& a, UniformControllerApprox const& b)
            -> bool;

private:
    int m_iterations      = 100;
    double m_yScale       = 1.0;
    bool m_fastMode       = false;
    bool m_renderLighting = true;

    glm::dvec3 m_colorOffset = {0.0, 1.0, 2.0};
    double m_colorFrequency  = 0.1;
};

#endif
