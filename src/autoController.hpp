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

#ifndef MANDELLANDSCAPE_AUTO_CONTROLLER_HPP
#define MANDELLANDSCAPE_AUTO_CONTROLLER_HPP

#include <functional>

#include <glm/glm.hpp>

#include "util.hpp"
#include "player.hpp"
#include "stateMap.hpp"

class AutoController final : public Player::Controller {
public:
    AutoController(std::function<double(glm::dvec2)>);

    auto
    update(Player::Internals& playerState, double dt) -> void final;

    auto
    handleMomentaryAction(MomentaryAction const&) -> void final;

    auto
    updateState(StateMap const& /*map*/) -> void final{};

private:
    auto
    locateTarget(Player::Internals const& playerState) -> void;

    std::function<double(glm::dvec2)> m_heightFunc;

    static auto constexpr maxTravelTime = 20.0;
    static auto constexpr minTravelTime = 7.0;
    static auto constexpr travelSpeed   = 1.0;

    glm::dvec2 m_target          = glm::dvec2{0.0, 0.0};
    double m_prevTargetDirection = 0.0;

    util::LowPassFilter<double> m_filteredLookAt{0.0, 0.99};

    bool m_needsRetarget = true;
};

#endif
