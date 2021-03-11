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

#include "util.hpp"

#include <string>
#include <fstream>

#include <glm/glm.hpp>

namespace util {

auto
readFile(std::string const& filePath) -> std::string
{
    std::string output;
    std::ifstream in(filePath);

    for(std::string line; std::getline(in, line);) {
        output += line + "\n";
    }

    in.close();
    return output;
}

auto
unitVec2(double theta) -> glm::dvec2
{
    return {std::cos(theta), std::sin(theta)};
}

}    // namespace util
