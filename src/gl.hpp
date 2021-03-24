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

#ifndef MANDELLANDSCAPE_GL_HPP
#define MANDELLANDSCAPE_GL_HPP

#include <utility>

#include <glad/glad.h>

namespace gl::destructor {

// CPP20 decltype(lambda)
class Fbo {
public:
    static auto
    destroy(GLuint address) noexcept -> void
    {
        glDeleteFramebuffers(1, &address);
    }
};

class Texture {
public:
    static auto
    destroy(GLuint address) noexcept -> void
    {
        glDeleteTextures(1, &address);
    }
};

struct Shader {
    static auto
    destroy(GLuint address) noexcept -> void
    {
        glDeleteShader(address);
    }
};

struct ShaderProgram {
    static auto
    destroy(GLuint address) noexcept -> void
    {
        glDeleteProgram(address);
    }
};

struct Vao {
    static auto
    destroy(GLuint address) noexcept -> void
    {
        glDeleteVertexArrays(1, &address);
    }
};

// Vbo and Ebo needs to be different types -> arbitrary differing template
// param
template<int>
struct Buffer {
    static auto
    destroy(GLuint address) noexcept -> void
    {
        glDeleteBuffers(1, &address);
    }
};
using Vbo = Buffer<__LINE__>;
using Ebo = Buffer<__LINE__>;

}    // namespace gl::destructor

namespace gl {

// RAII wrapper for OpenGL resources.
template<class Destructor>
class Resource {
public:
    Resource() noexcept = default;
    Resource(GLuint address) noexcept : m_address(address){};
    Resource(Resource<Destructor>&& other) noexcept
    {
        std::swap(m_address, other.m_address);
    }
    auto
    operator=(Resource<Destructor>&& other) noexcept -> Resource<Destructor>&
    {
        std::swap(m_address, other.m_address);
        return *this;
    }
    Resource(Resource<Destructor> const& other) = delete;

    // clang-format tries to align the two "="s
    // clang-format off
    auto
    operator=(Resource<Destructor> const& other)
        -> Resource<Destructor>& = delete;
    // clang-format on

    ~Resource() noexcept
    {
        Destructor::destroy(m_address);
    }

    operator GLuint&() noexcept
    {
        return m_address;
    }

    operator GLuint() const noexcept
    {
        return m_address;
    }

    auto
    operator&() noexcept -> GLuint*
    {
        return &m_address;
    }

    auto
    operator&() const noexcept -> GLuint const*
    {
        return &m_address;
    }

private:
    GLuint m_address = 0;
};

using Fbo           = Resource<destructor::Fbo>;
using Vao           = Resource<destructor::Vao>;
using Vbo           = Resource<destructor::Vbo>;
using Ebo           = Resource<destructor::Ebo>;
using Texture       = Resource<destructor::Texture>;
using Shader        = Resource<destructor::Shader>;
using ShaderProgram = Resource<destructor::ShaderProgram>;
}    // namespace gl

#endif
