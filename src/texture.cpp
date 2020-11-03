#include "texture.hpp"

#include <string>
#include <iostream>

#include <glad/glad.h>

#include <stb_image.h>

GLuint*
generateTexture()
{
    GLuint* texture = new GLuint;
    glGenTextures(1, texture);
    return texture;
}

Texture::Texture(std::string const& path, GLenum textureUnit) :
            m_location(generateTexture()),
            m_textureUnit(textureUnit)
{
    int width      = 0;
    int height     = 0;
    int nrChannels = 0;
    unsigned char* const image =
            stbi_load(path.c_str(), &width, &height, &nrChannels, 4);
    if(image == nullptr) {
        std::cerr << "Failed to load texture" << std::endl;
        throw;
    }

    glActiveTexture(m_textureUnit);
    glBindTexture(GL_TEXTURE_2D, *m_location);
    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image);
    glGenerateMipmap(GL_TEXTURE_2D);
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
