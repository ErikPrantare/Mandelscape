#include "shaderProgram.h"

#include <iostream>

#include <glm/glm.hpp>

GLuint*
createProgram()
{
    return new GLuint(glCreateProgram());
}

ShaderProgram::ShaderProgram() : m_location(createProgram())
{
    if(m_location == 0) {
        std::cerr << "Error creating shader program" << std::endl;
        throw;
    }
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
ShaderProgram::attachShader(GLuint const shader, GLenum const shaderType)
{
    if(shaderType == GL_VERTEX_SHADER) {
        attachVertexShader(*m_location, shader);
    }
    else {
        attachFragmentShader(*m_location, shader);
    }
}

void
ShaderProgram::compile()
{
    glLinkProgram(*m_location);

    GLint success = 0;

    glGetProgramiv(*m_location, GL_LINK_STATUS, &success);
    if(success == 0) {
        auto errorLog = std::array<GLchar, 1024>();
        glGetProgramInfoLog(
                *m_location,
                errorLog.size(),
                nullptr,
                errorLog.data());

        std::cerr << "Error linking shader program: " << errorLog.data()
                  << '\n';
        throw;
    }

    glValidateProgram(*m_location);
    glGetProgramiv(*m_location, GL_VALIDATE_STATUS, &success);
    if(success == 0) {
        auto errorLog = std::array<GLchar, 1024>();
        glGetProgramInfoLog(
                *m_location,
                errorLog.size(),
                nullptr,
                errorLog.data());

        std::cerr << "Invalid shader program: " << errorLog.data() << '\n';
        throw;
    }

    glUseProgram(*m_location);
}

void
ShaderProgram::setUniformMatrix4(
        std::string const& name,
        glm::mat4 const& value)
{
    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, &value[0][0]);
}

void
ShaderProgram::setUniformFloat(const std::string& name, float const x)
{
    glUniform1f(uniformLocation(name), x);
}

void
ShaderProgram::setUniformInt(const std::string& name, int const x)
{
    glUniform1i(uniformLocation(name), x);
}

void
ShaderProgram::setUniformVec2(
        const std::string& name,
        float const x,
        float const y)
{
    glUniform2f(uniformLocation(name), x, y);
}

GLuint
ShaderProgram::uniformLocation(std::string const& name) const
{
    GLuint location = glGetUniformLocation(*m_location, name.c_str());
    if(location == 0xFFFFFFFF) {
        std::cerr << "Failed to find variable " << name << std::endl;
        throw;
    }
    return location;
}
