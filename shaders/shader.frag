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
        PointInfo i = PointInfo(preCalculated, true);
        PointInfo o = PointInfo(preCalculated, false);
        fragColor =
            addFog(mix(color(i), color(o), 1.0-inside));
        return;
    }

    PointInfo p = value(makeComplex(position, offset));
    fragColor = addFog(color(p));

    vec3 lightDir = vec3(0.0, 0.0, 1.0);
    lightDir = normalize(lightDir);
    float illumination = 0.3;
    illumination += max(0.0, pow(
            dot(reflect(lightDir, normal), lookAt), 64.f));
    illumination += 0.5 * max(dot(lightDir, normal), 0);

    fragColor *= illumination;
}
