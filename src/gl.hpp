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

#ifndef MANDELLANDSCAPE_UTIL_GL_HPP
#define MANDELLANDSCAPE_UTIL_GL_HPP

#include <glad/glad.h>

namespace gl::destructor {

// CPP20 decltype(lambda)
class Fbo {
public:
    auto
    operator()(GLuint* location) noexcept -> void
    {
        glDeleteFramebuffers(1, location);
        delete location;
    }
};

class Texture {
public:
    auto
    operator()(GLuint* location) noexcept -> void
    {
        glDeleteTextures(1, location);
        delete location;
    }
};

struct Shader {
    auto
    operator()(GLuint const* location) noexcept -> void
    {
        glDeleteShader(*location);
        delete location;
    }
};

struct ShaderProgram {
    auto
    operator()(GLuint const* location) noexcept -> void
    {
        glDeleteProgram(*location);
        delete location;
    }
};

}    // namespace gl::destructor

#endif
