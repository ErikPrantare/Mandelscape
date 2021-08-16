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

#include "serialization.hpp"

template<>
auto
serialize<double>(
        std::ostream& out,
        double const& object,
        std::string const& name,
        int depth) -> void
{
    auto const indentation = std::string(depth, ' ');
    out << indentation << name << " = " << object;
}

auto
serialize(
        std::ostream& out,
        UniformController const& uniformController,
        std::string const& name) -> void
{
    out << name << " = {\n  ";
    serialize(out, uniformController.m_colorOffset, "colorOffset");
    out << ",\n  colorFrequency = " << uniformController.m_colorFrequency
        << ",\n  yScale = " << uniformController.m_yScale << ",\n  fastMode = "
        << (uniformController.m_fastMode ? "true" : "false")
        << ",\n  iterations = " << uniformController.m_iterations << "\n}\n\n";
}
