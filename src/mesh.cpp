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

#include "mesh.hpp"

#include <vector>

#include <glm/glm.hpp>

Mesh::Mesh()
{
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    glEnableVertexAttribArray(0);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    for(auto const& [location, vbo] : m_attributes) {
        glDeleteBuffers(1, &vbo);
    }
    glDeleteBuffers(1, &m_ebo);
}

auto
Mesh::render() -> void
{
    if(m_texture != nullptr) {
        m_texture->activate();
    }

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    for(auto const& [location, vbo] : m_attributes) {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(location, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    }

    glDrawElements(GL_TRIANGLES, m_nrVertices, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}

auto
Mesh::setVertices(std::vector<glm::vec3> const& vertices) -> void
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
            GL_ARRAY_BUFFER,
            vertices.size() * sizeof(vertices[0]),
            vertices.data(),
            GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

auto
Mesh::setVertices(
        std::vector<glm::vec3> const& vertices,
        int const start,
        int const size) -> void
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(
            GL_ARRAY_BUFFER,
            start * sizeof(vertices[0]),
            size * sizeof(vertices[0]),
            &vertices[start]);

    glBindVertexArray(0);
}

auto
Mesh::setIndices(std::vector<GLuint> const& indices) -> void
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            indices.size() * sizeof(indices[0]),
            indices.data(),
            GL_DYNAMIC_DRAW);

    m_nrVertices = indices.size();

    glBindVertexArray(0);
}

auto
Mesh::newAttribute(int location) -> void
{
    glBindVertexArray(m_vao);
    GLuint vbo = 0;
    glEnableVertexAttribArray(location);
    glGenBuffers(1, &vbo);

    glBindVertexArray(0);

    m_attributes[location] = vbo;
}

auto
Mesh::setTexture(std::shared_ptr<Texture> texture) -> void
{
    m_texture = std::move(texture);
}

auto
swap(Mesh& a, Mesh& b) -> void
{
    std::swap(a.m_vao, b.m_vao);
    std::swap(a.m_attributes, b.m_attributes);
    std::swap(a.m_vbo, b.m_vbo);
    std::swap(a.m_ebo, b.m_ebo);
    std::swap(a.m_nrVertices, b.m_nrVertices);
}
