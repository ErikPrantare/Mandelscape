#include "framebuffer.hpp"

#include <stdexcept>

Framebuffer::Framebuffer(glm::ivec2 size) noexcept(false) : m_size(size)
{
    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            m_size.x,
            m_size.y,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            nullptr);
    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            texture,
            0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_fbo);
}

auto
Framebuffer::size() const noexcept -> glm::ivec2
{
    return m_size;
}

auto
Framebuffer::bind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
}

auto
Framebuffer::unbind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
