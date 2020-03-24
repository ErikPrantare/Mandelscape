#version 300 es

layout(location = 0) in vec3 pos;

uniform mat4 cameraSpace;
uniform mat4 projection;

out mediump vec3 position;
out mediump float distance;

void
main()
{
    gl_Position = cameraSpace * vec4(pos, 1.0);
    position    = pos;
    distance    = sqrt(dot(gl_Position.xyz, gl_Position.xyz));

    gl_Position = projection * gl_Position;
}
