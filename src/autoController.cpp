#include "autoController.hpp"

#include <random>
#include <iostream>

#include "utils.hpp"

AutoController::AutoController(std::function<double(glm::dvec2)> heightFunc) :
            m_heightFunc(heightFunc){};

auto
AutoController::update(Player* const player, double const dt) -> void
{
    auto const relativePos = util::planePos(player->position);
    auto const offset      = util::planePos(player->positionOffset);
    auto const absolutePos = relativePos + offset;

    if(!m_targetFound) {
        auto rd = std::random_device();
        auto const distance =
                std::uniform_real_distribution<double>(2.0, 30.0)(rd);

        // minimize this function to find a proper goal
        auto fitness = [this,
                        &player,
                        distance](glm::dvec2 pos, double angle) -> double {
            if(m_heightFunc(pos) == 0.0) {
                return 1e99;
            }

            auto const angleDiff = std::abs(angle - m_prevTargetDirection);
            auto const anglePenalty =
                    -std::abs(angleDiff - util::pi) * distance / 3.0;
            return std::abs(m_heightFunc(pos) - player->scale) / player->scale
                   + anglePenalty;
        };

        auto bestTarget  = absolutePos;
        auto bestFitness = 1e99;
        auto bestAngle   = 0.0;

        auto const angleInit =
                std::uniform_real_distribution<double>(0.0, 0.01)(rd);
        for(double angle = angleInit; angle < 2.0 * util::pi; angle += 0.01) {
            auto const testTarget =
                    absolutePos
                    + distance * player->scale * util::unitVec2(angle);
            auto const testFitness = fitness(testTarget, angle);

            if(testFitness < bestFitness) {
                bestTarget  = testTarget;
                bestFitness = testFitness;
                bestAngle   = angle;
            }
        }

        m_prevTargetDirection = bestAngle;
        m_target              = bestTarget;
        m_targetFound         = true;
    }

    auto const distance = m_target - absolutePos;
    if(std::sqrt(glm::dot(distance, distance)) < dt * player->scale) {
        auto relativeTarget = m_target - offset;
        player->position.x  = relativeTarget.x;
        player->position.z  = relativeTarget.y;
        m_targetFound       = false;
        return;
    }

    auto const direction = glm::normalize(distance);
    player->position.x += dt * player->scale * direction.x;
    player->position.z += dt * player->scale * direction.y;

    // flipped in atan2 and +pi because offset is relative to -z and not x
    player->lookAtOffset.x =
            m_filteredLookAt(std::atan2(direction.x, direction.y) + util::pi);
    // look slightly down
    player->lookAtOffset.y = util::pi / 6;
}
