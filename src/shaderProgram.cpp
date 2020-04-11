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

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_location);
    glDeleteTextures(1, &m_textureLocation);
}

void
attachVertexShader(GLuint const program, GLuint const shader)
{
    static GLuint currentVertexShader = 0;
    glDetachShader(program, currentVertexShader);
    glAttachShader(program, shader);
    currentVertexShader = shader;
}

void
attachFragmentShader(GLuint const program, GLuint const shader)
{
    static GLuint currentFragmentShader = 0;
    glDetachShader(program, currentFragmentShader);
    glAttachShader(program, shader);
    currentFragmentShader = shader;
}

void
ShaderProgram::attatchShader(GLuint const shader, GLenum const shaderType) const
{
    if(shaderType == GL_VERTEX_SHADER)
        attachVertexShader(m_location, shader);
    else
        attachFragmentShader(m_location, shader);
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
    glUniformMatrix4fv(uniformLocation(name), 1, GL_TRUE, &value.m[0][0]);
}

void
ShaderProgram::setUniform(const std::string& name, float x, float y)
{
    glUniform2f(uniformLocation(name), x, y);
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
ShaderProgram::uniformLocation(const std::string& name) const
{
    GLuint location = glGetUniformLocation(m_location, name.c_str());
    if(location == 0xFFFFFFFF) {
        std::cerr << "Failed to find variable " << name << std::endl;
        throw;
    }
    return location;
}
