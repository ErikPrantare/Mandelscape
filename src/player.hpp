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

#include "momentaryAction.hpp"
#include "stateMap.hpp"
#include "util.hpp"

class Player {
public:
    struct Internals {
        Player* owner;
        glm::dvec3 position     = glm::dvec3{0.0, 0.0, 0.0};
        glm::dvec3 offset       = glm::dvec3{0.0, 0.0, 0.0};
        glm::dvec2 lookAtOffset = glm::dvec2{0.0, 0.0};
        double scale            = 1.0;
    };

    [[nodiscard]] auto
    truePosition() const noexcept -> glm::dvec3
    {
        return m_state.position + m_state.offset;
    }

    auto
    updateOffset(glm::dvec3 offset) noexcept -> void
    {
        auto dOffset = offset - m_state.offset;
        m_state.position -= dOffset;
        m_state.offset = offset;
    }

    [[nodiscard]] auto
    scale() const noexcept -> double
    {
        return m_state.scale;
    }

    [[nodiscard]] auto
    state() noexcept -> Internals&
    {
        return m_state;
    }

    [[nodiscard]] auto
    state() const noexcept -> Internals const&
    {
        return m_state;
    }

    auto
    updateFeetAltitude(double altitude) noexcept -> void
    {
        m_state.position.y = altitude;
    }

    friend auto
    util::lua::to<Player>(lua_State* L, int offset) -> Player;

    class Controller {
    public:
        virtual auto
        handleMomentaryAction(MomentaryAction const& action) -> void = 0;

        virtual auto
        updateState(StateMap const& map) -> void = 0;

        virtual auto
        update(Internals& player, double dt) -> void = 0;

        virtual ~Controller() = default;

        Controller()                  = default;
        Controller(Controller&&)      = default;
        Controller(Controller const&) = default;

        auto
        operator=(Controller&&) -> Controller& = default;
        auto
        operator=(Controller const&) -> Controller& = default;
    };

private:
    // CPP20 {.x = ...}
    Internals m_state{this};
};

#endif
