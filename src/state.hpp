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

#ifndef MANDELLANDSCAPE_PERSISTENTACTION_HPP
#define MANDELLANDSCAPE_PERSISTENTACTION_HPP

enum class State {
    MovingForwards,
    MovingBackwards,
    MovingLeft,
    MovingRight,

    RunningForwards,
    RunningBackwards,
    RunningLeft,
    RunningRight,

    ZoomingIn,
    ZoomingOut,

    IncreasingParameter,
    DecreasingParameter,

    ChangeRedOffset,
    ChangingGreenOffset,
    ChangingBlueOffset,
    ChangingTotalOffset,
    ChangingFrequency,
    ChangingYScale,
};

#endif
