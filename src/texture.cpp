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

Texture::Texture(CubemapArgs const& args) noexcept(false) :
            m_textureUnit(args.unit)
{
    GLenum textureUnit = args.unit;

    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureUnit);

    int width         = 0;
    int height        = 0;
    int nrChannels    = 0;
    auto const& faces = args.facePaths;
    for(unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data =
                stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if(data != nullptr) {
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0,
                    GL_RGB,
                    width,
                    height,
                    0,
                    GL_RGB,
                    GL_UNSIGNED_BYTE,
                    data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i]
                      << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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
