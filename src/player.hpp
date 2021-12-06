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

#ifndef MANDELSCAPE_PLAYER_HPP
#define MANDELSCAPE_PLAYER_HPP

#include <iostream>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "lua.h"

#include "camera.hpp"
#include "momentaryAction.hpp"
#include "serialization.hpp"
#include "stateMap.hpp"
#include "util.hpp"

class Player {
public:
    struct Internals {
        glm::dvec3 position     = glm::dvec3{0.0, 0.0, 0.0};
        glm::dvec3 offset       = glm::dvec3{0.0, 0.0, 0.0};
        glm::dvec2 lookAtOffset = glm::dvec2{0.0, 0.0};
        double scale            = 1.0;
    };

    auto
    update(double dt) -> void;

    [[nodiscard]] auto
    truePosition() const noexcept -> glm::dvec3;

    auto
    updateOffset(glm::dvec3 offset) noexcept -> void;

    [[nodiscard]] auto
    scale() const noexcept -> double;

    auto
    updateFeetAltitude(double altitude) noexcept -> void;

    [[nodiscard]] auto
    getCamera(glm::ivec2 viewSize) const noexcept -> Camera;

    [[nodiscard]] auto static getSerializationTable()
    {
        return std::make_tuple(
                makeMemberEntry<&Player::m_state, &Internals::scale>("scale"),
                makeMemberEntry<&Player::m_state, &Internals::position>(
                        "position"),
                makeMemberEntry<&Player::m_state, &Internals::offset>(
                        "offset"),
                makeMemberEntry<&Player::m_state, &Internals::lookAtOffset>(
                        "lookAtOffset"));
    }

    class Controller {
    public:
        virtual ~Controller() = default;

        Controller()                  = default;
        Controller(Controller&&)      = default;
        Controller(Controller const&) = default;

        auto
        operator=(Controller&&) -> Controller& = default;
        auto
        operator=(Controller const&) -> Controller& = default;

        virtual auto
        handleMomentaryAction(MomentaryAction const& action) -> void = 0;

        virtual auto
        updateState(StateMap const& map) -> void = 0;

        virtual auto
        update(Player& player, double dt) -> void = 0;

    protected:
        auto static internals(Player& p) -> Internals&
        {
            return p.m_state;
        }
        auto static internals(Player const& p) -> Internals const&
        {
            return p.m_state;
        }
    };

private:
    // CPP20 {.x = ...}
    Internals m_state;
    util::LowPassFilter<double> m_headAltitude{0.0, 0.01};
};

#endif
