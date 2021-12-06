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

#include <stdexcept>
#include <vector>
#include <memory>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gl.hpp"
#include "texture.hpp"

class Mesh {
public:
    Mesh();

    auto
    render(ShaderProgram& shaderProgram) -> void;

    auto
    setIndices(std::vector<GLuint> const& indices) -> void;

    template<class T>
    auto
    assertIsType(gl::AttributeInfo const info) noexcept(false)
    {
        if(gl::attributeInfo<T>() == info) {
            return true;
        }

        throw std::runtime_error("Error: Type mismatch in shader attribute");
    }

    template<class T>
    auto
    newAttribute(int location) -> void
    {
        glBindVertexArray(m_vao);
        GLuint vbo = 0;
        glEnableVertexAttribArray(location);
        glGenBuffers(1, &vbo);

        glBindVertexArray(0);

        // CPP20 {.x = ...}
        m_attributes[location].vbo  = vbo;
        m_attributes[location].info = gl::attributeInfo<T>();
    }

    template<class T>
    auto
    setAttribute(int location, std::vector<T> const& values) noexcept(false)
            -> void
    {
        assertIsType<T>(m_attributes.at(location).info);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_attributes[location].vbo);
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
            size_t size) noexcept(false) -> void
    {
        assertIsType<T>(m_attributes.at(location).info);

        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_attributes[location].vbo);
        glBufferSubData(
                GL_ARRAY_BUFFER,
                start * sizeof(T),
                size * sizeof(T),
                &values[start]);

        glBindVertexArray(0);
    }

    auto
    addTexture(std::shared_ptr<Texture> texture) -> void;

    friend auto
    swap(Mesh&, Mesh&) -> void;

private:
    struct Attribute {
        gl::Vbo vbo;
        gl::AttributeInfo info;
    };

    gl::Vao m_vao       = 0;
    gl::Ebo m_ebo       = 0;
    size_t m_nrVertices = 0;

    std::map<int, Attribute> m_attributes;

    std::vector<std::shared_ptr<Texture>> m_textures;
};

auto
swap(Mesh&, Mesh&) -> void;

#endif
