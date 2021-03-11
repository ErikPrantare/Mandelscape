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

#ifndef MANDELLANDSCAPE_CAMERA_HPP
#define MANDELLANDSCAPE_CAMERA_HPP
#include <tuple>

#include <glm/glm.hpp>

#include "util.hpp"

class Camera {
public:
    Camera() = delete;

    Camera(glm::dvec3 position,
           glm::dvec3 lookAt,
           glm::ivec2 viewSize,
           double scale);

    [[nodiscard]] auto
    projection() const -> glm::dmat4;

    [[nodiscard]] auto
    cameraSpace() const -> glm::dmat4;

private:
    [[nodiscard]] auto
    lookAtMatrix() const -> glm::dmat4;

    static double constexpr zNear  = 0.01;
    static double constexpr zFar   = 150.0;
    static double constexpr fov    = util::pi / 2;
    static glm::dvec3 constexpr up = {0.0, 1.0, 0.0};

    glm::ivec2 m_viewSize;
    glm::dvec3 m_pos;
    glm::dvec3 m_lookAt;
    double m_worldScale;
};

#endif
