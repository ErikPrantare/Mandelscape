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
    
    mediump vec2 c = pos.xz;
    mediump vec2 z = vec2(0.0, 0.0);

    color = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for(int i = 0; i < 30; ++i) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        if(dot(z, z) > 256.0f*256.0f) {
            mediump float colorVal = float(i) - log2(log2(dot(z, z)));
            color = (1.0f - distance/100.0f) * vec4(
                0.5f*sin(colorVal)+0.5f, 
                0.5f*sin(colorVal+1.0f)+0.5f, 
                0.5f*sin(colorVal+2.0f)+0.5f, 
                1.0f);
            return;
        }
    }
}
