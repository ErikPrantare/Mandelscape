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

#ifndef MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
#define MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP

#include <glm/vec2.hpp>

#include "event.hpp"
#include "player.hpp"
#include "momentaryAction.hpp"
#include "persistentActionMap.hpp"

class PlayerController {
public:
    virtual auto
    handleMomentaryAction(MomentaryAction const& action) -> void = 0;

    virtual auto
    updateState(PersistentActionMap const& map) -> void = 0;

    virtual auto
    update(Player* player, double dt) -> void = 0;

    virtual ~PlayerController() = default;

    PlayerController()                        = default;
    PlayerController(PlayerController&&)      = default;
    PlayerController(PlayerController const&) = default;

    auto
    operator=(PlayerController&&) -> PlayerController& = default;
    auto
    operator=(PlayerController const&) -> PlayerController& = default;
};

#endif    // MANDELLANDSCAPE_PLAYER_CONTROLLER_HPP
