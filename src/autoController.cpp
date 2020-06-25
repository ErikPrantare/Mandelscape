#include "autoController.hpp"

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
        auto fitness = [this, &player](glm::dvec2 x) {
            return std::abs(m_heightFunc(x) - player->scale);
        };

        auto constexpr distance = 100.0;
        auto bestTarget         = absolutePos;
        auto bestFitness        = 1e99;

        for(double angle = 0.0; angle < 2.0 * util::pi; angle += 0.01) {
            auto const testTarget =
                    absolutePos
                    + distance * player->scale * util::unitVec2(angle);
            auto const testFitness = fitness(testTarget);

            if(testFitness < bestFitness) {
                bestTarget  = testTarget;
                bestFitness = testFitness;
            }
        }

        m_target      = bestTarget;
        m_targetFound = true;
    }

    auto const distance = m_target - absolutePos;
    if(distance == glm::dvec2{0.0, 0.0}) {
        return;
    }

    auto const direction = glm::normalize(distance);
    player->position.x += dt * player->scale * direction.x;
    player->position.z += dt * player->scale * direction.y;

    // flipped in atan2 and +pi because offset is relative to -z and not x
    player->lookAtOffset.x = std::atan2(direction.x, direction.y) + util::pi;
    // look slightly down
    player->lookAtOffset.y = util::pi / 6;
}
