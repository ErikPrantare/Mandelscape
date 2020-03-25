#version 300 es

precision mediump float;

layout(location = 0) in vec3 pos;

uniform mat4 world;

out vec4 color;
out vec3 position;
out float distance;

void
main()
{
    gl_Position = world * vec4(pos, 1.0);
    color       = vec4(pos.y, pos.x, pos.z, 1.0);
    position    = pos;
    distance    = sqrt(dot(gl_Position.xyz, gl_Position.xyz));
}
