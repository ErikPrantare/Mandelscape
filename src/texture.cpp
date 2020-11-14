#include "texture.hpp"

#include <string>
#include <iostream>
#include <stdexcept>

#include <glad/glad.h>

#include <stb_image.h>

auto
generateTexture() -> GLuint*
{
    auto* const texture = new GLuint;
    glGenTextures(1, texture);
    return texture;
}

Texture::Texture(TextureArgs const& args) :
            m_location(generateTexture()),
            m_textureUnit(args.unit)
{
    unsigned char* image = nullptr;
    glm::ivec2 size      = args.size;

    if(args.imagePath != "") {
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
    glBindTexture(GL_TEXTURE_2D, *m_location);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            args.format,
            size.x,
            size.y,
            0,
            GL_RGBA,
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
    glBindTexture(GL_TEXTURE_2D, *m_location);
}
