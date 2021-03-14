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

#ifndef MANDELLANDSCAPE_PLAYER_HELPER_HPP
#define MANDELLANDSCAPE_PLAYER_HELPER_HPP

#include "player.hpp"

#include <type_traits>

// C++20 remove_cvref_t
template<
        class P,
        class = std::enable_if_t<std::is_same_v<Player&, std::decay_t<P>&>>>
class PlayerHelper {
public:
    PlayerHelper(P& player) noexcept : m_player(player)
    {}

    [[nodiscard]] auto
    truePosition() const noexcept -> glm::dvec3
    {
        return m_player.position + m_player.offset;
    }

    auto
    updateOffset(glm::dvec3 offset) noexcept -> void
    {
        auto dOffset = offset - m_player.offset;
        m_player.position -= dOffset;
        m_player.offset = offset;
    }

private:
    P& m_player;
};

#endif
