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

#ifndef MANDELSCAPE_GENERIC_CONTROLLER_HPP
#define MANDELSCAPE_GENERIC_CONTROLLER_HPP

#include <functional>

#include "util.hpp"
#include "momentaryAction.hpp"

class GenericController {
private:
    using MomentarySignature = void(MomentaryAction const&);

public:
    [[nodiscard]] auto
    withMomentary(std::function<MomentarySignature> f) const
            -> GenericController;

    auto
    handleMomentaryAction(MomentaryAction const& action) const -> void;

private:
    std::function<void(MomentaryAction const&)> m_handleMomentaryAction =
            util::nop;
};

#endif
