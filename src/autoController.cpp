#include "autoController.hpp"

#include <random>
#include <iostream>

#include "utils.hpp"

AutoController::AutoController(std::function<double(glm::dvec2)> heightFunc) :
            m_heightFunc(heightFunc){};

auto constexpr targetTooFar(
        double distanceToTarget,
        double maxTravelTime,
        double speed) -> bool
{
    return distanceToTarget / speed > maxTravelTime;
}

auto constexpr targetTooClose(
        double distanceToTarget,
        double speed,
        double deltaSeconds) -> bool
{
    auto const distanceTraveled = speed * deltaSeconds;

    return distanceToTarget < distanceTraveled;
}

auto
AutoController::update(Player& player, double const dt) -> void
{
    auto const relativePos = util::planePos(player.position);
    auto const offset      = util::planePos(player.positionOffset);
    auto const absolutePos = relativePos + offset;

    auto const distanceToTarget = glm::length(m_target - absolutePos);

    if(targetTooFar(distanceToTarget, maxTravelTime, player.scale * travelSpeed)
       || targetTooClose(distanceToTarget, player.scale * travelSpeed, dt)
       || m_target == glm::dvec2{0.0, 0.0}) {
        locateTarget(player);
    }

    auto const direction = glm::normalize(m_target - absolutePos);
    player.position.x += dt * player.scale * direction.x;
    player.position.z += dt * player.scale * direction.y;

    // flipped in atan2 and +pi because offset is relative to -z and not x
    player.lookAtOffset.x =
            m_filteredLookAt(std::atan2(direction.x, direction.y) + util::pi);
    // look slightly down
    player.lookAtOffset.y = util::pi / 6;
}

auto
AutoController::locateTarget(Player& player) -> void
{
    auto const relativePos = util::planePos(player.position);
    auto const offset      = util::planePos(player.positionOffset);
    auto const absolutePos = relativePos + offset;

    auto constexpr maxTravelDistance     = maxTravelTime / travelSpeed;
    auto constexpr minimumTravelDistance = minimumTravelTime / travelSpeed;

    auto rd             = std::random_device();
    auto const distance = std::uniform_real_distribution<double>(
            minimumTravelDistance,
            maxTravelDistance)(rd);

    // minimize this function to find a proper goal
    auto fitness =
            [this, &player, distance](glm::dvec2 pos, double angle) -> double {
        if(m_heightFunc(pos) == 0.0) {
            return 1e99;
        }

        // angleDiff is in [0, 2*pi]
        auto const angleDiff     = std::abs(angle - m_prevTargetDirection);
        auto const angleDistance = -std::abs(angleDiff - util::pi) + util::pi;
        auto const anglePenalty  = angleDistance * distance / 3.0;

        // Divide by player.scale to normalize height pos.
        auto const heightPenalty =
                std::abs(m_heightFunc(pos) / player.scale - 1.0);
        return heightPenalty + anglePenalty;
    };

    auto bestTarget  = absolutePos;
    auto bestFitness = 1e99;
    auto bestAngle   = 0.0;

    auto const angleInit =
            std::uniform_real_distribution<double>(0.0, 0.01)(rd);
    for(double angle = angleInit; angle < 2.0 * util::pi; angle += 0.01) {
        auto const testTarget =
                absolutePos + distance * player.scale * util::unitVec2(angle);
        auto const testFitness = fitness(testTarget, angle);

        if(testFitness < bestFitness) {
            bestTarget  = testTarget;
            bestFitness = testFitness;
            bestAngle   = angle;
        }
    }

    m_prevTargetDirection = bestAngle;
    m_target              = bestTarget;
}
