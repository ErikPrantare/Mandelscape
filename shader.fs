#version 300 es

in mediump vec4 color;
in mediump vec3 position;
in mediump float distance;
out mediump vec4 fragColor;

void main()
{
    mediump vec2 c = position.xz;
    mediump vec2 z = vec2(0.0, 0.0);

    fragColor = color;
    
    fragColor = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i < 30; ++i) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        if(dot(z, z) > 256.0f*256.0f) {
            mediump float colorVal = float(i) - log2(log2(dot(z, z)));
            fragColor = (1.0f - distance/100.0f) * vec4(
                0.5f*sin(colorVal)+0.5f, 
                0.5f*sin(colorVal+1.0f)+0.5f, 
                0.5f*sin(colorVal+2.0f)+0.5f, 
                1.0f);
            break;
        }
    }
}
