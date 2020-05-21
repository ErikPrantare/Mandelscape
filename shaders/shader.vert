#version 330

precision highp float;

layout(location = 0) in vec3 pos;

uniform mat4 cameraSpace;
uniform mat4 projection;
uniform vec2 offset;

out vec2 position;
out float distance;

void
main()
{
    gl_Position = cameraSpace * vec4(pos, 1.0);

    position = pos.xz;
    distance = sqrt(dot(gl_Position.xyz, gl_Position.xyz));

    gl_Position = projection * gl_Position;
}
