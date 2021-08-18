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

#include "autoController.hpp"

#include <random>
#include <iostream>
#include <variant>

#include <GLFW/glfw3.h>

#include "util.hpp"

AutoController::AutoController(std::function<double(glm::dvec2)> heightFunc) :
            m_heightFunc(std::move(heightFunc))
{}

auto
AutoController::handleMomentaryAction(MomentaryAction const& action) -> void
{
    // Controller was switched to
    if(std::holds_alternative<Trigger>(action)
       && std::get<Trigger>(action) == Trigger::ToggleAutoWalk) {
        m_needsRetarget = true;
    }
}

auto
AutoController::update(Player& player, double const dt) -> void
{
    auto const absolutePos = util::planePos(player.truePosition());

    auto const speed          = internals(player).scale * travelSpeed;
    auto const targetDistance = glm::length(m_target - absolutePos);

    m_needsRetarget = m_needsRetarget || targetDistance < dt * speed;

    if(m_needsRetarget) {
        locateTarget(player);
        m_needsRetarget = false;
    }

    auto const direction = glm::normalize(m_target - absolutePos);
    internals(player).position.x += dt * speed * direction.x;
    internals(player).position.z += dt * speed * direction.y;

    // flipped in atan2 and +pi because offset is relative to -z and not x
    m_filteredLookAt.update(std::atan2(direction.x, direction.y) + util::pi);
    internals(player).lookAtOffset.x = m_filteredLookAt.get();
    // look slightly down
    internals(player).lookAtOffset.y = util::pi / 6;
}

auto
AutoController::locateTarget(Player const& player) -> void
{
    using distribution = std::uniform_real_distribution<double>;

    auto const absolutePos = util::planePos(player.truePosition());

    auto rd               = std::random_device();
    auto const travelTime = distribution(minTravelTime, maxTravelTime)(rd);

    auto const anglePenalty = [this](double angle, double distance) -> double {
        // angleDiff is in [0, 2*pi]
        auto const angleDiff     = std::abs(angle - m_prevTargetDirection);
        auto const angleDistance = -std::abs(angleDiff - util::pi) + util::pi;
        return angleDistance * distance / 3.0;
    };

    auto const heightPenalty = [this, &player](glm::dvec2 pos) -> double {
        if(m_heightFunc(pos) == 0.0) {
            return 1e98;
        }

        // Divide by player.scale to normalize height pos.
        return std::abs(m_heightFunc(pos) / internals(player).scale - 1.0);
    };

    auto bestTarget  = absolutePos;
    auto bestPenalty = 1e99;
    auto bestAngle   = 0.0;

    auto const distance  = travelTime * travelSpeed;
    auto const angleInit = distribution(0.0, 0.01)(rd);

    for(double angle = angleInit; angle < 2.0 * util::pi; angle += 0.01) {
        auto const testTarget =
                absolutePos
                + distance * internals(player).scale * util::unitVec2(angle);
        auto const testPenalty =
                heightPenalty(testTarget) + anglePenalty(angle, distance);

        if(testPenalty < bestPenalty) {
            bestTarget  = testTarget;
            bestPenalty = testPenalty;
            bestAngle   = angle;
        }
    }

    m_prevTargetDirection = bestAngle;
    m_target              = bestTarget;
}
