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

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "glDestructors.hpp"

struct TextureArgs {
    glm::ivec2 size{0, 0};
    GLenum unit  = GL_TEXTURE0;
    GLint format = GL_RGBA;
    std::string imagePath;
    bool generateMipmap = false;
};

// OpenGL Texture2D
class Texture {
public:
    Texture(TextureArgs const& args) noexcept(false);

    auto
    activate() -> void;

    auto
    get() noexcept -> GLuint;

private:
    std::unique_ptr<GLuint, glDestructors::Texture> m_location;

    GLenum m_textureUnit;
};

#endif    // MANDELLANDSCAPE_TEXTURE_HPP
