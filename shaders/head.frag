#version 140

precision highp float;

in vec2 position;
in float distance;
in float preCalculated;
in float outside;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 offset;
uniform int iterations;
uniform float colorFrequency;
uniform vec3 colorOffset;
uniform bool fastMode;
