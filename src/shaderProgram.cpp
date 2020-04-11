#include "shaderProgram.h"

#include <iostream>

ShaderProgram::ShaderProgram() : m_location(glCreateProgram())
{
    if(m_location == 0) {
        std::cerr << "Error creating shader program" << std::endl;
        throw;
    }
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(m_location);
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
ShaderProgram::compile() const
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
ShaderProgram::setUniform(const std::string& name, const GLuint x) const
{
    glUniform1i(uniformLocation(name), x);
}

void
ShaderProgram::setUniform(const std::string& name, const Matrix4f& value) const
{
    glUniformMatrix4fv(uniformLocation(name), 1, GL_TRUE, &value.m[0][0]);
}

void
ShaderProgram::setUniform(const std::string& name, float x, float y) const
{
    glUniform2f(uniformLocation(name), x, y);
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
