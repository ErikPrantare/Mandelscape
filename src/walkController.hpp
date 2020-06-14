#ifndef MANDELLANDSCAPE_WALKCONTROLLER_HPP
#define MANDELLANDSCAPE_WALKCONTROLLER_HPP

#include <glm/glm.hpp>

#include "event.hpp"
#include "playerController.hpp"

class WalkController : public PlayerController {
public:
    auto
    handleEvent(Event const&) -> void override;

    auto
    update(Player* player, double dt) -> void override;

private:
    static double constexpr movementSpeed = 1.0;
    glm::dvec3 m_direction{0.0, 0.0, 0.0};
    glm::dvec2 m_dLookAtOffset{0.0, 0.0};
};

#endif
