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

#ifndef MANDELLANDSCAPE_EVENT_HPP
#define MANDELLANDSCAPE_EVENT_HPP

#include <variant>

#include "glfwEnums.hpp"
#include "glm/glm.hpp"

struct KeyDown {
    Input::Key const code = {};
    int const mods        = 0;
};

struct KeyUp {
    Input::Key const code;
    int const mods;
};

struct MouseMove {
    glm::dvec2 pos;
    glm::dvec2 dPos;
};

struct MouseButtonDown {
    Input::MouseButton const code;
};

struct MouseButtonUp {
    Input::MouseButton const code;
};

using Event = std::
        variant<KeyUp, KeyDown, MouseMove, MouseButtonUp, MouseButtonDown>;

// C++20: operator<=>
auto
operator<(KeyDown a, KeyDown b) -> bool;
auto
operator<(KeyUp a, KeyUp b) -> bool;
auto
operator<(MouseMove a, MouseMove b) -> bool;
auto
operator<(MouseButtonUp a, MouseButtonUp b) -> bool;
auto
operator<(MouseButtonDown a, MouseButtonDown b) -> bool;

#endif
