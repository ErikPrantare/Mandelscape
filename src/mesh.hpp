#ifndef MANDELLANDSCAPE_MESH_HPP
#define MANDELLANDSCAPE_MESH_HPP

#include <glad/glad.h>

class Mesh {
public:
    Mesh();
    Mesh(Mesh const&) = delete;
    Mesh&
    operator=(Mesh const&) = delete;
    Mesh(Mesh&&)           = delete;
    Mesh&
    operator=(Mesh&&) = delete;

    ~Mesh();
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;

private:
};

#endif
