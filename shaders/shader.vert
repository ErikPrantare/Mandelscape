#version 140

precision highp float;

in vec3 pos;
in float value;
in int inside_;

uniform mat4 cameraSpace;
uniform mat4 projection;
uniform vec2 offset;
uniform float yScale;

out vec2 position;
out float distance;
out float preCalculated;
out float inside;

void
main()
{
    gl_Position = cameraSpace * vec4(pos.x, yScale*pos.y, pos.z, 1.0);

    position = pos.xz;
    distance = sqrt(dot(gl_Position.xyz, gl_Position.xyz));

    preCalculated = value;
    inside = inside_ != 0 ? 1.0 : 0.0;

    gl_Position = projection * gl_Position;
}
