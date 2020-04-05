#ifndef MANDELLANDSCAPE_SHADERPROGRAM_H
#define MANDELLANDSCAPE_SHADERPROGRAM_H

#include <map>
#include <GL/glew.h>

#include "shader.h"
#include "math3d.h"

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    ShaderProgram(const ShaderProgram&) = delete;
    ShaderProgram
    operator=(const ShaderProgram&) = delete;

    void
    useShader(const Shader& shader);

    void
    compile();

    void
    setUniform(const std::string& name, const Matrix4f& value);

    void
    setUniform(const std::string& name, float, float);

    void
    setTexture(
            unsigned char* const image,
            int width,
            int height,
            int nrChannels);

    void
    bindTexture();

private:
    GLuint m_location;
    GLuint m_textureLocation;
    bool m_textureLoaded = false;

    GLuint
    locationOf(const std::string& name) const;

    std::map<GLenum, GLuint> m_shaders;
};

#endif
