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

#include "shaderProgram.hpp"

#include <iostream>
#include <array>
#include <stdexcept>

#include <glm/glm.hpp>

ShaderProgram::ShaderProgram() noexcept(false) :
            m_location(new GLuint(glCreateProgram()))
{
    if(*m_location == 0) {
        throw std::runtime_error{"Error creating shader program"};
    }
}

static void
attachVertexShader(GLuint const program, GLuint const shader)
{
    static GLuint currentVertexShader = 0;
    glDetachShader(program, currentVertexShader);
    glAttachShader(program, shader);
    currentVertexShader = shader;
}

static void
attachFragmentShader(GLuint const program, GLuint const shader)
{
    static GLuint currentFragmentShader = 0;
    glDetachShader(program, currentFragmentShader);
    glAttachShader(program, shader);
    currentFragmentShader = shader;
}

void
ShaderProgram::attachShader(GLuint const shader, GLenum const shaderType)
{
    if(shaderType == GL_VERTEX_SHADER) {
        attachVertexShader(*m_location, shader);
    }
    else {
        attachFragmentShader(*m_location, shader);
    }
}

void
ShaderProgram::compile()
{
    glLinkProgram(*m_location);

    GLint success = 0;

    glGetProgramiv(*m_location, GL_LINK_STATUS, &success);
    if(success == 0) {
        auto errorLog = std::array<GLchar, 1024>();
        glGetProgramInfoLog(
                *m_location,
                errorLog.size(),
                nullptr,
                errorLog.data());

        throw std::runtime_error(
                std::string{"Error linking shader program: "}
                + errorLog.data());
    }

    glValidateProgram(*m_location);
    glGetProgramiv(*m_location, GL_VALIDATE_STATUS, &success);
    if(success == 0) {
        auto errorLog = std::array<GLchar, 1024>();
        glGetProgramInfoLog(
                *m_location,
                errorLog.size(),
                nullptr,
                errorLog.data());

        throw std::runtime_error(
                std::string{"Invalid shader program: "} + errorLog.data());
    }

    glUseProgram(*m_location);
}

void
ShaderProgram::bindAttributeLocation(std::string const& name, int const index)
{
    glBindAttribLocation(*m_location, index, name.c_str());
}

void
ShaderProgram::setUniformMatrix4(
        std::string const& name,
        glm::mat4 const& value)
{
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void
ShaderProgram::setUniformFloat(const std::string& name, float const x)
{
    glUniform1f(uniformLocation(name), x);
}

void
ShaderProgram::setUniformInt(const std::string& name, int const x)
{
    glUniform1i(uniformLocation(name), x);
}

void
ShaderProgram::setUniformVec2(const std::string& name, glm::vec2 v)
{
    glUniform2f(uniformLocation(name), v.x, v.y);
}

void
ShaderProgram::setUniformVec3(const std::string& name, glm::vec3 v)
{
    glUniform3f(uniformLocation(name), v.x, v.y, v.z);
}

auto
ShaderProgram::uniformLocation(std::string const& name) const -> GLuint
{
    // It's ok if name doesn't exist.
    // This will then return 0xFFFFFF,
    // and setting that adress will have no effect
    return glGetUniformLocation(*m_location, name.c_str());
}
