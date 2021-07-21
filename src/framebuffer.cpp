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

#include "framebuffer.hpp"

#include <stdexcept>

static auto
textureArgs(glm::ivec2 size) noexcept -> TextureArgs
{
    // CPP20 {.a = b}
    TextureArgs args;
    args.size   = size;
    args.format = GL_RGB;
    return args;
}

static auto
depthArgs(glm::ivec2 size) noexcept -> TextureArgs
{
    // CPP20 {.a = b}
    TextureArgs args;
    args.size   = size;
    args.format = GL_DEPTH_COMPONENT;
    return args;
}

Framebuffer::Framebuffer(glm::ivec2 size) noexcept(false) :
            m_texture(textureArgs(size)),
            m_depth(depthArgs(size)),
            m_size(size)
{
    glGenFramebuffers(1, &m_fbo);
    bind();

    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_texture.get(),
            0);

    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            m_depth.get(),
            0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }
}

[[nodiscard]] auto
Framebuffer::readPixels() -> std::vector<unsigned char>
{
    bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    // CPP23 size_t suffix (3z)
    std::vector<unsigned char> pixels(
            3 * static_cast<size_t>(m_size.x) * m_size.y);

    glReadPixels(
            0,
            0,
            m_size.x,
            m_size.y,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            pixels.data());

    return pixels;
}

auto
Framebuffer::size() const noexcept -> glm::ivec2
{
    return m_size;
}

auto
Framebuffer::bind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

auto
Framebuffer::unbind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
