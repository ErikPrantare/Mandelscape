#version 300 es

layout (location = 0) in vec3 pos;

uniform mat4 world;

out mediump vec4 color;
out mediump vec3 position;
out mediump float distance;

void main()
{
    gl_Position = world * vec4(pos, 1.0);
    color = vec4(pos.y, pos.x, pos.z, 1.0);
    position = pos;
    distance = gl_Position.w;
}
