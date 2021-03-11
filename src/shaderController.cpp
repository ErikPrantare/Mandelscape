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
ShaderController::updateState(
        PersistentActionMap const& active,
        double const dt) -> void
{
    int direction = (active(PersistentAction::IncreaseParam) ? 1 : 0)
                    - (active(PersistentAction::DecreaseParam) ? 1 : 0);

    if(active(PersistentAction::ChangeFrequency)) {
        m_colorFrequency *= std::exp(direction * dt);
    }
    if(active(PersistentAction::ChangeRedOffset)) {
        m_colorOffset.x += 3 * direction * dt;
    }
    if(active(PersistentAction::ChangeGreenOffset)) {
        m_colorOffset.y += 3 * direction * dt;
    }
    if(active(PersistentAction::ChangeBlueOffset)) {
        m_colorOffset.z += 3 * direction * dt;
    }
    if(active(PersistentAction::ChangeTotalOffset)) {
        m_colorOffset += 3 * direction * dt * glm::dvec3{1.0, 1.0, 1.0};
    }
    if(active(PersistentAction::ChangeYScale)) {
        if(m_yScale > 1.0) {
            m_yScale *= std::exp(direction * dt);
        }
        else if(m_yScale < -1.0) {
            m_yScale *= std::exp(-direction * dt);
        }
        else {
            m_yScale += direction * dt;
        }
    }
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

    shaderProgram->setUniformInt("fastMode", static_cast<int>(m_fastMode));
    shaderProgram->setUniformFloat(
            "colorFrequency",
            static_cast<float>(m_colorFrequency));
    shaderProgram->setUniformVec3("colorOffset", m_colorOffset);
    shaderProgram->setUniformFloat("yScale", static_cast<float>(m_yScale));
}

auto
ShaderController::yScale() const -> double
{
    return m_yScale;
}
