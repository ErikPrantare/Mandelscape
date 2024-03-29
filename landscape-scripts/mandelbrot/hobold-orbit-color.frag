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

#line 18 1

vec4
color(const in PointInfo p)
{
    if(p.inside) return vec4(0.0, 0.0, 0.0, 1.0);

    vec3 colorVal = floor(p.value) * colorFrequency + colorOffset;

    vec4 color =
        pow(p.value - floor(p.value), 0.5)
        * vec4(0.5 * sin(colorVal) + 0.5, 1.0);

    return color;
}
