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

    glGenBuffers(1, &m_ebo);

    glBindVertexArray(0);
}

auto
Mesh::render(ShaderProgram& shaderProgram) -> void
{
    for(auto& texture : m_textures) {
        texture->activateOn(shaderProgram);
    }

    glBindVertexArray(m_vao);

    for(auto const& [location, attribute] : m_attributes) {
        glBindBuffer(GL_ARRAY_BUFFER, attribute.vbo);
        glVertexAttribPointer(
                location,
                attribute.info.nrComponents,
                attribute.info.type,
                GL_FALSE,
                0,
                nullptr);
    }

    glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(m_nrVertices),
            GL_UNSIGNED_INT,
            nullptr);

    glBindVertexArray(0);
}

auto
Mesh::setIndices(std::vector<GLuint> const& indices) -> void
{
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLintptr>(indices.size() * sizeof(indices[0])),
            indices.data(),
            GL_DYNAMIC_DRAW);

    m_nrVertices = indices.size();

    glBindVertexArray(0);
}

auto
Mesh::addTexture(std::shared_ptr<Texture> texture) -> void
{
    m_textures.emplace_back(std::move(texture));
}

auto
swap(Mesh& a, Mesh& b) -> void
{
    std::swap(a.m_vao, b.m_vao);
    std::swap(a.m_attributes, b.m_attributes);
    std::swap(a.m_ebo, b.m_ebo);
    std::swap(a.m_nrVertices, b.m_nrVertices);
    std::swap(a.m_textures, b.m_textures);
}
