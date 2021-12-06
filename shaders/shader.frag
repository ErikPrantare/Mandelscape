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

#line 18 2

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

float
illumination(in vec3 lightDir)
{
    lightDir = normalize(lightDir);

    float ambient = 0.2;

    float diffuse = max(0.0, dot(lightDir, normal));

    vec3 lookingDirection = normalize(playerPos - worldPosition);
    vec3 reflection = reflect(-lightDir, normal);
    float specular =
        0.5 * pow(max(dot(lookingDirection, reflection), 0.0), 32);

    return ambient + diffuse + specular;
}

vec4
mirrorColor()
{
    vec3 lookingDirection = normalize(playerPos - worldPosition);
    vec3 reflection = reflect(-lookingDirection, normal);
    vec4 color = texture(skybox, reflection);
    return vec4(vec3(0.33*(color.r+color.g+color.b)), 1.0);
}

vec4
addLighting(const in vec4 color)
{
    if(!renderLighting) {
        return color;
    }

    float illumination = pow(illumination(vec3(0.0, 1.0, 0.0)), 1.0/2.2);
    
    vec4 terrainColor = clamp(vec4(0.0), vec4(1.0), illumination * color);
    float reflectionStrength = 0.1;

    return mix(terrainColor, mirrorColor(), reflectionStrength);
}

void
main()
{
    if(fastMode) {
        PointInfo i = PointInfo(preCalculated, true);
        PointInfo o = PointInfo(preCalculated, false);
        fragColor =
            addFog(addLighting(
                    mix(color(i), color(o), 1.0-inside)));
        return;
    }

    PointInfo p = value(makeComplex(position, offset));
    fragColor = addFog(addLighting(color(p)));
}
