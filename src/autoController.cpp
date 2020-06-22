#include "autoController.hpp"

auto
AutoController::update(Player* const player, double const dt) -> void
{
    player->position += dt * player->scale
                        * glm::dvec3{
                                std::cos(player->lookAtOffset.x),
                                0.0,
                                -std::sin(player->lookAtOffset.x)};
}
