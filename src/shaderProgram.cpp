#include "shaderProgram.h"

#include <iostream>

#include <stb_image.h>

ShaderProgram::ShaderProgram() : m_location(glCreateProgram())
{
    if(m_location == 0) {
        std::cerr << "Error creating shader program" << std::endl;
        throw;
    }
    glGenTextures(1, &m_textureLocation);
}

void
ShaderProgram::useShader(const Shader& shader)
{
    if(m_shaders.count(shader.m_type)) {
        glDetachShader(m_location, m_shaders[shader.m_type]);
    }

    glAttachShader(m_location, shader.m_location);
    m_shaders[shader.m_type] = shader.m_location;
}

void
ShaderProgram::compile()
{
    glLinkProgram(m_location);

    GLint success         = 0;
    GLchar errorLog[1024] = {0};

    glGetProgramiv(m_location, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_location, sizeof(errorLog), nullptr, errorLog);

        std::cerr << "Error linking shader program: " << errorLog << std::endl;
        throw;
    }

    glValidateProgram(m_location);
    glGetProgramiv(m_location, GL_VALIDATE_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(m_location, sizeof(errorLog), nullptr, errorLog);

        std::cerr << "Invalid shader program: " << errorLog << std::endl;
        throw;
    }

    glUseProgram(m_location);
}

void
ShaderProgram::setUniform(const std::string& name, const Matrix4f& value)
{
    glUniformMatrix4fv(locationOf(name), 1, GL_TRUE, &value.m[0][0]);
}

void
ShaderProgram::setUniform(const std::string& name, float x, float y)
{
    glUniform2f(locationOf(name), x, y);
}

void
ShaderProgram::setTexture(
        unsigned char* const image,
        int width,
        int height,
        int nrChannels)
{
    glBindTexture(GL_TEXTURE_2D, m_textureLocation);
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
}

void
ShaderProgram::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, m_textureLocation);
}

GLuint
ShaderProgram::locationOf(const std::string& name) const
{
    GLuint loc = glGetUniformLocation(m_location, name.c_str());
    if(loc == 0xFFFFFFFF) {
        std::cerr << "Failed to find variable " << name << std::endl;
        throw;
    }
    return loc;
}
