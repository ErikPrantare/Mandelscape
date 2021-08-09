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

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <stb_image_write.h>
#include <stb_image_resize.h>

#include "util.hpp"

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

static auto
savePng(std::filesystem::path const& path,
        std::vector<unsigned char> const& pixels,
        glm::ivec2 const size) -> void
{
    namespace fs          = std::filesystem;
    std::string const dir = "screenshots";
    if(!fs::exists(path.parent_path())
       || !fs::is_directory(path.parent_path())) {
        fs::create_directory(path.parent_path());
    }

    stbi_flip_vertically_on_write(1);
    stbi_write_png(path.c_str(), size.x, size.y, 3, pixels.data(), 0);
}

auto
Framebuffer::savePngDownsampled(std::filesystem::path const& path) -> void
{
    auto const renderedSize    = size();
    auto const antiAliasedSize = renderedSize / 2;
    auto const pixels          = readPixels();

    // CPP23 3z * outputSize.x
    std::vector<unsigned char> antiAliasedImage(
            3 * static_cast<long>(antiAliasedSize.x) * antiAliasedSize.y);

    stbir_resize_uint8(
            pixels.data(),
            renderedSize.x,
            renderedSize.y,
            0,
            antiAliasedImage.data(),
            antiAliasedSize.x,
            antiAliasedSize.y,
            0,
            3);

    savePng(path, antiAliasedImage, antiAliasedSize);
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
Framebuffer::bindDefaultBuffer() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
