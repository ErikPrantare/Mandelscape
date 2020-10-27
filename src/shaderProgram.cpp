#include "shaderProgram.hpp"

#include <iostream>
#include <array>

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
ShaderProgram::setUniformVec2(const std::string& name, glm::vec2 v)
{
    glUniform2f(uniformLocation(name), v.x, v.y);
}

void
ShaderProgram::setUniformVec3(const std::string& name, glm::vec3 v)
{
    glUniform3f(uniformLocation(name), v.x, v.y, v.z);
}

GLuint
ShaderProgram::uniformLocation(std::string const& name) const
{
    // It's ok if name doesn't exist.
    // This will then return 0xFFFFFF,
    // and setting that adress will have no effect
    return glGetUniformLocation(*m_location, name.c_str());
}
