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

#include "walkController.hpp"

#include <algorithm>

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "util.hpp"

auto
WalkController::update(Player& player, double const dt) -> void
{
    auto constexpr pi = glm::pi<double>();

    internals(player).lookAtOffset += m_dLookAtOffset;
    internals(player).lookAtOffset.y = std::clamp(
            internals(player).lookAtOffset.y,
            -pi / 2 + 0.001,
            pi / 2 - 0.001);
    m_dLookAtOffset = glm::dvec2{0.0, 0.0};

    auto rotator = glm::rotate(
            glm::dmat4(1.0),
            internals(player).lookAtOffset.x,
            {0.0, 1.0, 0.0});
    internals(player).position +=
            dt * internals(player).scale * movementSpeed
            * glm::dvec3(rotator * glm::dvec4(m_direction, 1.0));

    if(!m_autoZoom) {
        internals(player).scale *= std::exp(dt * m_scalingVelocity);
    }
    else if(internals(player).position.y != 0.0) {
        internals(player).scale = internals(player).position.y;
    }
}

auto
WalkController::handleMomentaryAction(MomentaryAction const& action) -> void
{
    std::visit(
            util::Overload{
                    [this](Trigger action) {
                        if(action == Trigger::ToggleAutoZoom) {
                            m_autoZoom = !m_autoZoom;
                        }
                        else if(action == Trigger::ToggleAutoWalk) {
                            // Don't autozoom if previously autowalking
                            m_autoZoom = false;
                        }
                    },

                    [this](MouseDelta mouse) {
                        m_dLookAtOffset += util::pixelsToAngle(mouse.dPos);
                    },

                    // default
                    util::nop},
            action);
}
auto
WalkController::updateState(StateMap const& active) -> void
{
    m_direction = {0, 0, 0};
    if(active(State::MovingForwards)) {
        m_direction.z -= 1.0;
    }
    if(active(State::MovingBackwards)) {
        m_direction.z += 1.0;
    }
    if(active(State::MovingLeft)) {
        m_direction.x -= 1.0;
    }
    if(active(State::MovingRight)) {
        m_direction.x += 1.0;
    }

    if(active(State::RunningForwards)) {
        m_direction.z -= 2.0;
    }
    if(active(State::RunningBackwards)) {
        m_direction.z += 2.0;
    }
    if(active(State::RunningLeft)) {
        m_direction.x -= 2.0;
    }
    if(active(State::RunningRight)) {
        m_direction.x += 2.0;
    }

    m_scalingVelocity = 0.0;
    if(active(State::ZoomingIn)) {
        m_scalingVelocity -= 1.0;
    }
    if(active(State::ZoomingOut)) {
        m_scalingVelocity += 1.0;
    }
}
