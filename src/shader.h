#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>

#include <GL/glew.h>

class Shader {
public:
    static Shader
    fromFile(const std::string& filename, GLenum shaderType);

    static Shader
    fromCode(const std::string& code, GLenum shaderType);

private:
    Shader() = default;
    friend class ShaderProgram;

    GLenum m_type;
    GLuint m_location;
};

#endif
