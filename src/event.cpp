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

#include "event.hpp"

#include <tuple>

auto
operator<(KeyDown a, KeyDown b) -> bool
{
    return std::tie(a.code, a.mods) < std::tie(b.code, b.mods);
}
auto
operator<(KeyUp a, KeyUp b) -> bool
{
    return std::tie(a.code, a.mods) < std::tie(b.code, b.mods);
}
auto
operator<(MouseMove a, MouseMove b) -> bool
{
    return std::tie(a.x, a.y, a.dx, a.dy) < std::tie(b.x, b.y, b.dx, b.dy);
}
auto
operator<(MouseButtonUp a, MouseButtonUp b) -> bool
{
    return a.code < b.code;
}
auto
operator<(MouseButtonDown a, MouseButtonDown b) -> bool
{
    return a.code < b.code;
}
