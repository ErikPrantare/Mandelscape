#ifndef MANDELLANDSCAPE_WALKCONTROLLER_HPP
#define MANDELLANDSCAPE_WALKCONTROLLER_HPP

#include "event.hpp"
#include "player.hpp"

#include <glm/glm.hpp>

class WalkController {
public:
    auto
    update(Player& player, double dt) -> void;

    auto
    handleEvent(Event const&) -> void;

private:
    static double constexpr movementSpeed = 1.0;

    double m_scalingVelocity = 0.0;
    bool m_autoZoom          = false;
    glm::dvec3 m_direction{0.0, 0.0, 0.0};
    glm::dvec2 m_dLookAtOffset{0.0, 0.0};
};

#endif
