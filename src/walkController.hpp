#ifndef MANDELLANDSCAPE_WALKCONTROLLER_HPP
#define MANDELLANDSCAPE_WALKCONTROLLER_HPP

#include <glm/glm.hpp>

#include "event.hpp"
#include "player.hpp"
#include "playerController.hpp"
#include "momentaryAction.hpp"
#include "persistentActionMap.hpp"

class WalkController final : public PlayerController {
public:
    auto
    handleMomentaryAction(MomentaryAction const&) -> void final;

    auto
    updateState(PersistentActionMap const& active) -> void final;

    auto
    update(Player* player, double dt) -> void final;

private:
    static double constexpr movementSpeed = 1.0;

    double m_scalingVelocity = 0.0;
    bool m_autoZoom          = false;
    glm::dvec3 m_direction{0.0, 0.0, 0.0};
    glm::dvec2 m_dLookAtOffset{0.0, 0.0};
};

#endif
