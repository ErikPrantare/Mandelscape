#include "autoController.hpp"

#include "utils.hpp"

AutoController::AutoController(std::function<double(glm::dvec2)> heightFunc) :
            m_heightFunc(heightFunc){};

auto
AutoController::update(Player* const player, double const dt) -> void
{
    auto playerPos2d = glm::dvec2{player->position.x, player->position.z};
    auto offset =
            glm::dvec2{player->positionOffset.x, player->positionOffset.z};
    auto bestPosition = playerPos2d;
    auto bestHeight   = m_heightFunc(bestPosition + offset);

    for(double t = 0.0; t < 2.0 * util::pi; t += 0.02) {
        auto testPos =
                playerPos2d
                + player->scale * dt * glm::dvec2{std::sin(t), std::cos(t)};
        auto testHeight = m_heightFunc(testPos + offset);
        if(testHeight < bestHeight) {
            bestPosition = testPos;
            bestHeight   = testHeight;
        }
    }

    player->position.x = bestPosition.x;
    player->position.z = bestPosition.y;
}
