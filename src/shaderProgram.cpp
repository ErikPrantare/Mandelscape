#include "shaderProgram.h"

#include <iostream>

#include <stb_image.h>

ShaderProgram::ShaderProgram(
        std::string const& vertexShader,
        std::string const& fragmentShader) :
            m_location(glCreateProgram())
{
    if(m_location == 0) {
        std::cerr << "Error creating shader program" << std::endl;
        throw;
    }
    glGenTextures(1, &m_textureLocation);

    attatchShader(vertexShader, GL_VERTEX_SHADER);
    attatchShader(fragmentShader, GL_FRAGMENT_SHADER);
    compile();
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_location);
    glDeleteTextures(1, &m_textureLocation);
}

GLuint
ShaderProgram::getLocation(std::string const& path, GLenum const shaderType)
{
    if(m_shaders.find(path) == std::end(m_shaders))
        m_shaders.emplace(path, Shader::fromFile(path, shaderType));

    return m_shaders.find(path)->second.location();
}
void
ShaderProgram::attatchShader(std::string const& path, GLenum const shaderType)
{
    static GLuint currentVertexShader   = 0;
    static GLuint currentFragmentShader = 0;

    switch(shaderType) {
    case GL_VERTEX_SHADER: {
        glDetachShader(m_location, currentVertexShader);
        currentVertexShader = getLocation(path, shaderType);
        glAttachShader(m_location, currentVertexShader);
    } break;

    case GL_FRAGMENT_SHADER: {
        glDetachShader(m_location, currentFragmentShader);
        currentFragmentShader = getLocation(path, shaderType);
        glAttachShader(m_location, currentFragmentShader);
    } break;
    }
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
