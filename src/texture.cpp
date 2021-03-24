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

#include "texture.hpp"

#include <string>
#include <iostream>
#include <stdexcept>

#include <glad/glad.h>

#include <stb_image.h>

auto
generateTexture() -> GLuint
{
    auto texture = GLuint();
    glGenTextures(1, &texture);
    return texture;
}

Texture::Texture(TextureArgs const& args) :
            m_address(generateTexture()),
            m_textureUnit(args.unit)
{
    unsigned char* image = nullptr;
    glm::ivec2 size      = args.size;

    if(!args.imagePath.empty()) {
        int nrChannels = 0;
        image          = stbi_load(
                args.imagePath.c_str(),
                &size.x,
                &size.y,
                &nrChannels,
                4);

        if(image == nullptr) {
            throw std::runtime_error("Failed to load texture image");
        }
    }

    glActiveTexture(m_textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_address);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            args.format,
            size.x,
            size.y,
            0,
            args.format,
            GL_UNSIGNED_BYTE,
            image);

    if(args.generateMipmap) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
}

auto
Texture::activate() -> void
{
    glActiveTexture(m_textureUnit);
    glBindTexture(GL_TEXTURE_2D, m_address);
}

auto
Texture::get() noexcept -> GLuint
{
    return m_address;
}
