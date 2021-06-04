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

#ifndef MANDELLANDSCAPE_SHADERPROGRAM_HPP
#define MANDELLANDSCAPE_SHADERPROGRAM_HPP

#include <map>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gl.hpp"

class ShaderProgram {
public:
    ShaderProgram() noexcept(false);

    auto
    attachShader(GLuint shader, GLenum shaderType) -> void;

    auto
    compile() noexcept(false) -> void;

    auto
    bindAttributeLocation(std::string const& name, int index) -> void;

    auto
    setUniformFloat(std::string const& name, float) -> void;

    auto
    setUniformUInt(std::string const& name, unsigned int) -> void;

    auto
    setUniformInt(std::string const& name, int) -> void;

    auto
    setUniformVec2(std::string const& name, glm::vec2) -> void;

    auto
    setUniformVec3(std::string const& name, glm::vec3) -> void;

    auto
    setUniformMatrix4(std::string const& name, glm::mat4 const& value) -> void;

private:
    gl::ShaderProgram m_address;

    [[nodiscard]] auto
    uniformLocation(const std::string& name) const -> GLint;

    [[nodiscard]] auto
    getLocation(std::string const& path, GLenum shaderType) const -> GLuint;
};

#endif
