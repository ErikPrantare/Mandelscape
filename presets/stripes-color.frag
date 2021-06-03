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

    vec3 colorVal = p.value * colorFrequency + colorOffset;

    //val + 0.1 avoids weird color glitches in when val is integer
    vec4 color = texture(stripes, vec2(0.0, p.value + 0.1))
            * vec4(0.5f * sin(colorVal) + 0.5f, 1.0f);

    return color;
}
