/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    // Todo: RAII to remove destructor and = delete
    ~Mesh();

    auto
    render() -> void;

    auto
    setVertices(std::vector<glm::vec3> const& vertices) -> void;

    auto
    setVertices(
            std::vector<glm::vec3> const& vertices,
            size_t start,
            size_t size) -> void;

    auto
    setIndices(std::vector<GLuint> const& indices) -> void;

    auto
    newAttribute(int location) -> void;

    template<class T>
    auto
    setAttribute(int location, std::vector<T> const& values) -> void
    {
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_attributes[location]);
        glBufferData(
                GL_ARRAY_BUFFER,
                values.size() * sizeof(T),
                values.data(),
                GL_DYNAMIC_DRAW);

        glBindVertexArray(0);
    }

    template<class T>
    auto
    setAttribute(
            int location,
            std::vector<T> const& values,
            size_t start,
            size_t size) -> void
    {
        glBindVertexArray(m_vao);

        glBindBuffer(GL_ARRAY_BUFFER, m_attributes[location]);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                start * sizeof(T),
                size * sizeof(T),
                &values[start]);

        glBindVertexArray(0);
    }

    auto
    setTexture(std::shared_ptr<Texture> texture) -> void;

    friend auto
    swap(Mesh&, Mesh&) -> void;

    static int constexpr vertexLocation = 0;

private:
    GLuint m_vao        = 0;
    GLuint m_vbo        = 0;
    GLuint m_ebo        = 0;
    size_t m_nrVertices = 0;

    // location -> VBO
    std::map<int, GLuint> m_attributes = {};

    std::shared_ptr<Texture> m_texture = nullptr;
};

auto
swap(Mesh&, Mesh&) -> void;

#endif
