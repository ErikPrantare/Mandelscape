#include "framebuffer.hpp"

#include <stdexcept>

static auto
textureArgs(glm::ivec2 size) noexcept -> TextureArgs
{
    // CPP20 {.a = b}
    TextureArgs args;
    args.size   = size;
    args.format = GL_RGB;
    return args;
}

static auto
depthArgs(glm::ivec2 size) noexcept -> TextureArgs
{
    // CPP20 {.a = b}
    TextureArgs args;
    args.size   = size;
    args.format = GL_DEPTH_COMPONENT;
    return args;
}

Framebuffer::Framebuffer(glm::ivec2 size) noexcept(false) :
            m_texture(textureArgs(size)),
            m_depth(depthArgs(size)),
            m_size(size)
{
    glGenFramebuffers(1, m_fbo.get());
    bind();

    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            m_texture.get(),
            0);

    glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            m_depth.get(),
            0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw std::runtime_error("Failed to create framebuffer");
    }
}

[[nodiscard]] auto
Framebuffer::readPixels() -> std::vector<unsigned char>
{
    bind();
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);

    std::vector<unsigned char> pixels(3 * m_size.x * m_size.y);

    glReadPixels(
            0,
            0,
            m_size.x,
            m_size.y,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            pixels.data());

    return pixels;
}

auto
Framebuffer::size() const noexcept -> glm::ivec2
{
    return m_size;
}

auto
Framebuffer::bind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, *m_fbo);
}

auto
Framebuffer::unbind() noexcept -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
