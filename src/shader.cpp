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

#include "shader.hpp"

#include <string>
#include <iostream>
#include <array>
#include <stdexcept>

#include "util.hpp"

template<ShaderType type>
auto
Shader<type>::createShader(std::string const& sourceCode) noexcept(false)
        -> GLuint*
{
    auto const shaderType = static_cast<GLenum>(type);
    auto* const location  = new GLuint(glCreateShader(shaderType));

    if(*location == 0) {
        auto typeStr = shaderType == GL_FRAGMENT_SHADER ? std::string("FRAG")
                                                        : std::string("VERT");

        throw std::runtime_error{"Error creating shader of type " + typeStr};
    }

    GLchar const* charSource = sourceCode.c_str();
    auto const shaderLength  = static_cast<GLint>(sourceCode.length());

    glShaderSource(*location, 1, &charSource, &shaderLength);
    glCompileShader(*location);

    GLint success = 0;
    glGetShaderiv(*location, GL_COMPILE_STATUS, &success);
    if(success == 0) {
        auto infoLog = std::array<GLchar, 1024>();
        glGetShaderInfoLog(
                *location,
                sizeof(infoLog),
                nullptr,
                infoLog.data());
        auto typeStr = shaderType == GL_FRAGMENT_SHADER ? std::string("FRAG")
                                                        : std::string("VERT");

        throw std::runtime_error(
                std::string{"Error compiling shader of type "} + typeStr + ": "
                + "'" + infoLog.data() + "'");
    }

    return location;
}

template<ShaderType type>
Shader<type>::Shader(std::string const& sourceCode) noexcept(false) :
            m_location(createShader(sourceCode))
{}

template<ShaderType type>
auto
Shader<type>::fromFile(std::string const& filePath) noexcept(false)
        -> Shader<type>
{
    return Shader<type>(util::getContents(std::ifstream(filePath)));
}

template<ShaderType type>
auto
Shader<type>::fromCode(std::string const& sourceCode) noexcept(false)
        -> Shader<type>
{
    return Shader<type>(sourceCode);
}

template<ShaderType type>
void
Shader<type>::attachTo(ShaderProgram& program) const
{
    program.attachShader(*m_location, static_cast<GLenum>(type));
}

template class Shader<ShaderType::Vertex>;
template class Shader<ShaderType::Fragment>;
