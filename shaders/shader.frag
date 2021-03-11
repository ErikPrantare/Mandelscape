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

vec4
addFog(const in vec4 color)
{
    float fogHardStart = 100.0;
    float fogHardEnd   = 150.0;
    float fog          = 1.0 - pow(0.98, distance);
    
    fog += clamp(
            (distance - fogHardStart) / (fogHardEnd - fogHardStart),
            0.0,
            1.0);
    
    fog = pow(fog, 2.0);

    return vec4(fog, fog, fog, 1.0) + (1.0 - fog) * color;
}

void
main()
{
    if(fastMode) {
        fragColor =
            addFog(
                inside * color(preCalculated, true)
                + (1.0 - inside) * color(preCalculated, false));
        return;
    }

    bool inside;
    float val = value(makeComplex(position, offset), inside);
    fragColor = addFog(color(val, inside));
}
