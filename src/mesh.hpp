#ifndef MANDELLANDSCAPE_MESH_HPP
#define MANDELLANDSCAPE_MESH_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

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

    auto
    render() -> void;

    auto
    setVertices(std::vector<glm::vec3> const& vertices) -> void;

    auto
    setVertices(std::vector<glm::vec3> const& vertices, int start, int size)
            -> void;

    auto
    setIndices(std::vector<GLuint> const& indices) -> void;

    friend auto
    swap(Mesh&, Mesh&) -> void;

private:
    GLuint m_VAO = 0;
    GLuint m_VBO = 0;
    GLuint m_EBO = 0;

    int m_nrVertices = 0;
};

auto
swap(Mesh&, Mesh&) -> void;

#endif
