#include "shaderController.hpp"

#include <variant>

#include "util.hpp"

ShaderController::ShaderController(ShaderProgram* shaderProgram)
{
    m_vertexShader.attachTo(*shaderProgram);
    m_fragmentShaders[m_currentFragmentShader].attachTo(*shaderProgram);
    shaderProgram->compile();
}

auto
ShaderController::handleMomentaryAction(MomentaryAction const& action) -> void
{
    auto onTrigger = [this](TriggerAction action) {
        switch(action) {
        case TriggerAction::SwitchShader: {
            m_currentFragmentShader++;
            m_currentFragmentShader %= m_fragmentShaders.size();
            m_switchShader = true;
        } break;
        case TriggerAction::ToggleFastMode: {
            m_fastMode = !m_fastMode;
        } break;
        default:
            break;
        }
    };

    std::visit(util::Overload{onTrigger, util::unaryNOP}, action);
}

auto
ShaderController::update(ShaderProgram* const shaderProgram) -> void
{
    if(m_switchShader) {
        m_switchShader = false;

        m_fragmentShaders[m_currentFragmentShader].attachTo(*shaderProgram);
        shaderProgram->compile();
    }

    shaderProgram->setUniformInt("fastMode", m_fastMode);
}
