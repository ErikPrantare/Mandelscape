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

#ifndef MANDELLANDSCAPE_WALKCONTROLLER_HPP
#define MANDELLANDSCAPE_WALKCONTROLLER_HPP

#include <glm/glm.hpp>

#include "event.hpp"
#include "player.hpp"
#include "playerController.hpp"
#include "momentaryAction.hpp"
#include "stateMap.hpp"

class WalkController final : public PlayerController {
public:
    auto
    handleMomentaryAction(MomentaryAction const&) -> void final;

    auto
    updateState(StateMap const& active) -> void final;

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
