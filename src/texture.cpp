#include "texture.h"

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

Texture::Texture(std::string const& path) : m_location(generateTexture())
{
    int width;
    int height;
    int nrChannels;
    unsigned char* const image =
            stbi_load("textures/texture.png", &width, &height, &nrChannels, 4);
    if(image == nullptr) {
        std::cerr << "Failed to load texture" << std::endl;
        throw;
    }

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

void
Texture::makeActiveOn(GLenum const textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, *m_location);
}
