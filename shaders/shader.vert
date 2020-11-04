#version 140

precision highp float;

in vec3 pos;
in float colorValue;

uniform mat4 cameraSpace;
uniform mat4 projection;
uniform vec2 offset;

out vec2 position;
out float distance;
out float preCalculated;

void
main()
{
    gl_Position = cameraSpace * vec4(pos, 1.0);

    position = pos.xz;
    distance = sqrt(dot(gl_Position.xyz, gl_Position.xyz));
    preCalculated = colorValue;

    gl_Position = projection * gl_Position;
}
