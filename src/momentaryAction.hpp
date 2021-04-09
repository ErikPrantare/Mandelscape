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

#ifndef MANDELLANDSCAPE_MOMENTARYACTION_HPP
#define MANDELLANDSCAPE_MOMENTARYACTION_HPP

#include <variant>

enum class Trigger {
    IncreaseIterations,
    DecreaseIterations,
    SwitchShader,
    TakeScreenshot,
    CloseWindow,

    ToggleAutoWalk,
    ToggleAutoZoom,
    ToggleFastMode,
    TogglePause,
    ToggleLighting,

    Save,
    Load,

    LoadTerrainFunctions,
};

struct MouseDelta {
    double dx;
    double dy;
};

using MomentaryAction = std::variant<Trigger, MouseDelta>;

// CPP20: <=> operator
auto
operator==(MouseDelta a, MouseDelta b) -> bool;

auto
operator<(MouseDelta a, MouseDelta b) -> bool;

auto
sameAction(MomentaryAction const& action, Trigger triggerAction) -> bool;

#endif    // MANDELLANDSCAPE_MOMENTARYACTION_HPP
