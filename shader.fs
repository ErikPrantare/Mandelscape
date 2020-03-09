#version 300 es

in mediump vec4 color;
in mediump vec3 position;
out mediump vec4 fragColor;

void main()
{
    mediump vec2 c = position.xz;
    mediump vec2 z = vec2(0.0, 0.0);
    
    fragColor = vec4(0.0, 0.0, 0.0, 0.0);

    for(int i = 0; i < 30; ++i) {
        z = vec2(z.x*z.x - z.y*z.y, 2.0*z.x*z.y) + c;
        if(z.x*z.x + z.y*z.y > 4.0) {
            fragColor = vec4(
                0.5f*sin(float(i))+0.5f, 
                0.5f*sin(float(i)+1.0f)+0.5f, 
                0.5f*sin(float(i)+2.0f)+0.5f, 
                1.0f);
            break;
        }
    }
}
