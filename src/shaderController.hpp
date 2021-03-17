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

#ifndef MANDELSCAPE_SHADERCONTROLLER_HPP
#define MANDELSCAPE_SHADERCONTROLLER_HPP

#include <glm/glm.hpp>

#include "persistentActionMap.hpp"
#include "momentaryAction.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"

/* Manage which shaders to use */
class ShaderController {
public:
    ShaderController(ShaderProgram* shaderProgram);

    auto
    handleMomentaryAction(MomentaryAction const&) -> void;

    auto
    update(ShaderProgram* shaderProgram) -> void;

private:
    VertexShader m_vertexShader =
            VertexShader::fromFile("shaders/shader.vert");

    bool m_switchShader                             = false;
    size_t m_currentFragmentShader                  = 0;
    std::array<FragmentShader, 2> m_fragmentShaders = {
            FragmentShader::fromFiles(
                    "shaders/head.frag",
                    "shaders/shallowLib.frag",
                    "shaders/value.frag",
                    "shaders/color.frag",
                    "shaders/shader.frag"),
            FragmentShader::fromFiles(
                    "shaders/head.frag",
                    "shaders/deepLib.frag",
                    "shaders/value.frag",
                    "shaders/color.frag",
                    "shaders/shader.frag")};
};

#endif
