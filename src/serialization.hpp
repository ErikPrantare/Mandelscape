/* Mandelscape
 * Copyright (C) 2021 Erik Präntare
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

#ifndef MANDELSCAPE_SERIALIZATION_HPP
#define MANDELSCAPE_SERIALIZATION_HPP

#include <ostream>

#include "lua.hpp"

#include "player.hpp"
#include "uniformController.hpp"

auto
serialize(std::ostream&, Player const&, std::string const& name) -> void;

auto
serialize(std::ostream&, UniformController const&, std::string const& name)
        -> void;

#endif
