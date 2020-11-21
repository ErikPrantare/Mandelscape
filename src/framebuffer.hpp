#ifndef MANDELSCAPE_FRAMEBUFFER_HPP
#define MANDELSCAPE_FRAMEBUFFER_HPP

#include <memory>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.hpp"
#include "glDestructors.hpp"

class Framebuffer {
public:
    Framebuffer(glm::ivec2 size) noexcept(false);

    [[nodiscard]] auto
    size() const noexcept -> glm::ivec2;

    auto
    bind() noexcept -> void;

    auto
    unbind() noexcept -> void;

private:
    std::unique_ptr<GLuint, glDestructors::Fbo> m_fbo{new GLuint};
    Texture m_texture;
    Texture m_depth;
    glm::ivec2 m_size;
};

#endif
