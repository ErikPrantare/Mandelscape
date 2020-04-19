#version 300 es

precision highp float;

in vec3 position;
in float distance;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 offset;
uniform float time;

vec2
complexMultiplication(const in vec2 a, const in vec2 b)
{
    return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

void
main()
{
    float fogHardStart = 100.0;
    float fogHardEnd   = 150.0;
    float fog          = 1.0 - pow(0.98, distance);
    
    fog += clamp(
            (distance - fogHardStart) / (fogHardEnd - fogHardStart),
            0.0,
            1.0);
    
    fog       = pow(fog, 2.0);
    fragColor = vec4(fog, fog, fog, 1.0);

    vec2 c = position.xz + offset;

    // main cardioid check
    float q = pow(c.x - 0.25f, 2.0f) + c.y * c.y;
    if(q * (q + (c.x - 0.25f)) < 0.25f * c.y * c.y) {
        //return;
    }

    // period-2 bulb check
    if((c.x + 1.0f) * (c.x + 1.0f) + c.y * c.y < 0.25f * 0.25f) {
        //return;
    }

    vec2 z   = vec2(0.0, 0.0);
    float dist = 1e10;

    vec2 point = 3.0*vec2(sin(time*0.1684)*sin(time), cos(time));
    float radius = 1.0 + sin(time*0.286);

    for(int i = 0; i < 100; ++i) {
        z = complexMultiplication(z, z) + c;
        dist = min(dist, abs(dot(z - point, z - point) - radius));
        if(dot(z, z) > 256.0f * 256.0f) {
            float colorVal = float(i) - log2(log2(dot(z, z)));
            colorVal += 1.0 * dist - 1.0*time;
            fragColor =
                    fog * vec4(1.0, 1.0, 1.0, 1.0)
                    + (1.0 - fog) * texture(tex, vec2(0.0, colorVal))
                    * vec4(
                              0.5f * sin(colorVal * 0.1f) + 0.5f,
                              0.5f * sin(colorVal * 0.13f + 1.0f) + 0.5f,
                              0.5f * sin(colorVal * 0.15f + 2.0f) + 0.5f,
                              1.0f);
            return;
        }
    }
            float colorVal = float(100) - log2(log2(dot(z, z)));
            colorVal = 1.0 * dist - 1.0*time;
            fragColor =
                    fog * vec4(1.0, 1.0, 1.0, 1.0)
                    + (1.0 - fog) * texture(tex, vec2(0.0, colorVal))
                    * vec4(
                              0.5f * sin(colorVal * 0.1f) + 0.5f,
                              0.5f * sin(colorVal * 0.13f + 1.0f) + 0.5f,
                              0.5f * sin(colorVal * 0.15f + 2.0f) + 0.5f,
                              1.0f);
}
