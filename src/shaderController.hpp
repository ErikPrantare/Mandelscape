#ifndef MANDELSCAPE_SHADERCONTROLLER_HPP
#define MANDELSCAPE_SHADERCONTROLLER_HPP

#include "momentaryAction.hpp"
#include "shader.hpp"
#include "shaderProgram.hpp"

class ShaderController {
public:
    ShaderController(ShaderProgram* shaderProgram);

    auto
    handleMomentaryAction(MomentaryAction const&) -> void;

    auto
    update(ShaderProgram* shaderProgram) -> void;

private:
    VertexShader m_vertexShader =
            VertexShader::fromFile("shaders/shader.vert");

    int m_currentFragmentShader                     = 0;
    std::array<FragmentShader, 2> m_fragmentShaders = {
            FragmentShader::fromFile("shaders/shader.frag"),
            FragmentShader::fromFile("shaders/deepShader.frag")};

    bool m_fastMode     = false;
    bool m_switchShader = false;
};

#endif
