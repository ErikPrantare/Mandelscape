#ifndef MANDELLANDSCAPE_MESH_HPP
#define MANDELLANDSCAPE_MESH_HPP

#include <vector>
#include <memory>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"

class Mesh {
public:
    Mesh();

    Mesh(Mesh const&) = delete;
    auto
    operator=(Mesh const&) -> Mesh& = delete;
    Mesh(Mesh&&)                    = delete;
    auto operator=(Mesh &&) -> Mesh& = delete;

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

    // CPP20 nodiscard("Index of created attribute array")]
    auto
    newAttribute(int location) -> void;

    auto
    setAttribute(int index, std::vector<float> const& values) -> void;

    auto
    setAttribute(
            int index,
            std::vector<float> const& values,
            int start,
            int size) -> void;

    auto
    setTexture(std::shared_ptr<Texture> texture) -> void;

    friend auto
    swap(Mesh&, Mesh&) -> void;

private:
    GLuint m_VAO     = 0;
    GLuint m_VBO     = 0;
    GLuint m_EBO     = 0;
    int m_nrVertices = 0;

    // location -> VBO
    std::map<int, GLuint> m_attributes = {};

    std::shared_ptr<Texture> m_texture = nullptr;
};

auto
swap(Mesh&, Mesh&) -> void;

#endif
