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

#ifndef MANDELLANDSCAPE_TEXTURE_HPP
#define MANDELLANDSCAPE_TEXTURE_HPP

#include <string>
#include <memory>
#include <array>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gl.hpp"
#include "shaderProgram.hpp"

struct TextureArgs {
    glm::ivec2 size{0, 0};
    GLint index  = 0;
    GLint format = GL_RGBA;
    std::string imagePath;
    bool generateMipmap = false;
    std::string uniformName;
};

struct CubemapArgs {
    std::array<std::string, 6> facePaths;
    GLint index = 0;
    std::string uniformName;
};

// OpenGL Texture2D
class Texture {
public:
    Texture(TextureArgs const& args) noexcept(false);
    Texture(CubemapArgs const& args) noexcept(false);

    auto
    activateOn(ShaderProgram& shaderProgram) -> void;

    auto
    get() noexcept -> GLuint;

private:
    auto
    bind() -> void;

    gl::Texture m_address;
    GLint m_index;
    GLenum m_type;
    std::string m_uniformName;
};

#endif    // MANDELLANDSCAPE_TEXTURE_HPP
