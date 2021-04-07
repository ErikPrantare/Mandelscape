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

#version 140

precision highp float;

in vec3 pos;
in float value;
in int inside_;
in vec3 normal_;

uniform mat4 cameraSpace;
uniform mat4 projection;
uniform vec2 offset;
uniform float yScale;

out vec2 position;
out float distance;
out float preCalculated;
out float inside;
out vec3 normal;

void
main()
{
    gl_Position = cameraSpace * vec4(pos.x, yScale*pos.y, pos.z, 1.0);

    position = pos.xz;
    distance = sqrt(dot(gl_Position.xyz, gl_Position.xyz));

    preCalculated = value;
    inside = inside_ != 0 ? 1.0 : 0.0;
    normal = normalize(normal_);

    gl_Position = projection * gl_Position;
}
