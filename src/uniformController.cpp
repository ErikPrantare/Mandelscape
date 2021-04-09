/* Mandelscape
 * Copyright (C) 2021 Erik Präntare
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

#include "uniformController.hpp"
#include "util.hpp"

auto
UniformController::updateState(
        PersistentActionMap const& active,
        double const dt) noexcept -> void
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
UniformController::handleMomentaryAction(MomentaryAction const& action) -> void
{
    auto onTrigger = [this](Trigger action) {
        switch(action) {
        case Trigger::IncreaseIterations: {
            m_iterations += 20;
        } break;
        case Trigger::DecreaseIterations: {
            m_iterations -= 20;
        } break;
        case Trigger::ToggleFastMode: {
            m_fastMode = !m_fastMode;
        } break;
        case Trigger::ToggleLighting: {
            m_lighting = !m_lighting;
        } break;
        default:
            break;
        }
    };

    std::visit(util::Overload{onTrigger, util::nop}, action);
}

auto
UniformController::yScale() const noexcept -> double
{
    return m_yScale;
}

auto
UniformController::iterations() const noexcept -> int
{
    return m_iterations;
}

auto
UniformController::update(ShaderProgram* const shaderProgram) -> void
{
    shaderProgram->setUniformInt("fastMode", static_cast<int>(m_fastMode));
    shaderProgram->setUniformInt("lighting", static_cast<int>(m_lighting));
    shaderProgram->setUniformFloat(
            "colorFrequency",
            static_cast<float>(m_colorFrequency));
    shaderProgram->setUniformVec3("colorOffset", m_colorOffset);
    shaderProgram->setUniformFloat("yScale", static_cast<float>(m_yScale));
}

auto
UniformController::operator==(UniformController const& other) const noexcept
        -> bool
{
    return std::tie(m_fastMode, m_colorFrequency, m_colorOffset, m_yScale)
           == std::tie(
                   other.m_fastMode,
                   other.m_colorFrequency,
                   other.m_colorOffset,
                   other.m_yScale);
}
