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

#ifndef MANDELSCAPE_FRAMEBUFFER_HPP
#define MANDELSCAPE_FRAMEBUFFER_HPP

#include <memory>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "gl.hpp"

class Framebuffer {
public:
    Framebuffer(glm::ivec2 size) noexcept(false);

    [[nodiscard]] auto
    size() const noexcept -> glm::ivec2;

    [[nodiscard]] auto
    readPixels() -> std::vector<unsigned char>;

    auto
    bind() noexcept -> void;

    static auto
    unbind() noexcept -> void;

private:
    std::unique_ptr<GLuint, gl::destructor::Fbo> m_fbo{new GLuint};
    Texture m_texture;
    Texture m_depth;
    glm::ivec2 m_size;
};

#endif
