#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>

#include <GL/glew.h>

class Shader {
public:
    void
    loadFromFile(const std::string& filename, GLenum shaderType);

    void
    loadFromCode(const std::string& code, GLenum shaderType);

private:
    friend class ShaderProgram;

    GLenum m_type;
    GLuint m_location;
};

#endif
