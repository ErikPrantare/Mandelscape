#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <string>

#include <GL/glew.h>

class Shader {
public:
    Shader()              = delete;
    Shader(const Shader&) = delete;
    Shader&
    operator=(const Shader&) = delete;

    ~Shader();

    static Shader
    fromFile(const std::string& filename, GLenum shaderType);

    static Shader
    fromCode(const std::string& code, GLenum shaderType);

private:
    Shader(GLenum location, GLenum type);
    friend class ShaderProgram;

    GLenum m_type;
    GLuint m_location;
};

#endif
