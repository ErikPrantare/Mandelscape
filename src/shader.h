#ifndef MANDELLANDSCAPE_SHADER_H
#define MANDELLANDSCAPE_SHADER_H

#include <GL/glew.h>

class Shader {
public:
    void
    setProgram(GLuint program);

    void
    addShader(const std::string& shaderCode, GLenum shaderType);

private:
    GLuint m_shaderProgram;
};

#endif
