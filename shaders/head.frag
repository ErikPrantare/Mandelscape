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

in vec2 position;
in float distance;
in float preCalculated;
in float inside;
in vec3 normal;
in vec3 worldPosition;
out vec4 fragColor;

uniform sampler2D stripes;
uniform samplerCube skybox;

uniform vec2 offset;
uniform int iterations;
uniform float colorFrequency;
uniform vec3 colorOffset;

uniform bool fastMode;
uniform bool renderLighting;

uniform float time;
uniform vec3 lookAt;
uniform vec3 playerPos;

struct PointInfo {
    float value;
    bool inside;
};
