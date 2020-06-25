#include "autoController.hpp"

#include <cmath>

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
        // minimize this function to find a proper goal
        auto fitness = [this, &player](glm::dvec2 pos, double angle) -> double {
            if(m_heightFunc(pos) == 0.0) {
                return 1e99;
            }

            return std::abs(m_heightFunc(pos) - player->scale) / player->scale
                   + std::fmod(angle - m_prevTargetDirection, 2 * util::pi);
        };

        auto constexpr distance = 40.0;
        auto bestTarget         = absolutePos;
        auto bestFitness        = 1e99;

        for(double angle = 0.0; angle < 2.0 * util::pi; angle += 0.01) {
            auto const testTarget =
                    absolutePos
                    + distance * player->scale * util::unitVec2(angle);
            auto const testFitness = fitness(testTarget, angle);

            if(testFitness < bestFitness) {
                bestTarget            = testTarget;
                bestFitness           = testFitness;
                m_prevTargetDirection = angle;
            }
        }

        m_target      = bestTarget;
        m_targetFound = true;
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
