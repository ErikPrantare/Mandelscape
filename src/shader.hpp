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

#ifndef MANDELLANDSCAPE_SHADER_HPP
#define MANDELLANDSCAPE_SHADER_HPP

#include <string>
#include <memory>
#include <iostream>
#include <type_traits>

#include <glad/glad.h>

#include "shaderProgram.hpp"
#include "gl.hpp"
#include "util.hpp"

enum class ShaderType {
    Vertex   = GL_VERTEX_SHADER,
    Fragment = GL_FRAGMENT_SHADER
};

template<ShaderType type>
class Shader {
public:
    Shader() = delete;

    [[nodiscard]] static auto
    fromFile(std::string const& filePath) noexcept(false) -> Shader;

    template<class... Args>
    [[nodiscard]] static auto
    fromFiles(Args... filePaths) noexcept(false) -> Shader
    {
        return Shader<type>(
                (util::getContents(std::ifstream(filePaths)) + ...));
    }

    [[nodiscard]] static auto
    fromCode(std::string const& sourceCode) noexcept(false) -> Shader;

    auto
    attachTo(ShaderProgram& program) const -> void;

private:
    Shader(std::string const& sourceCode);

    gl::Shader m_location;

    [[nodiscard]] static auto
    createShader(std::string const& sourceCode) noexcept(false)
            -> GLuint*;
};

using VertexShader   = Shader<ShaderType::Vertex>;
using FragmentShader = Shader<ShaderType::Fragment>;

#endif
