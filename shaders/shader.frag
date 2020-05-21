#version 330 core

precision highp float;

in vec2 position;
in float distance;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 offset;
uniform int iterations;

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

    vec2 c = position + offset;

    // main cardioid check
    float q = pow(c.x - 0.25f, 2.0f) + c.y * c.y;
    if(q * (q + (c.x - 0.25f)) < 0.25f * c.y * c.y) {
        return;
    }

    // period-2 bulb check
    if((c.x + 1.0f) * (c.x + 1.0f) + c.y * c.y < 0.25f * 0.25f) {
        return;
    }

    vec2 z   = vec2(0.0, 0.0);

    for(int i = 0; i < iterations; ++i) {
        z = complexMultiplication(z, z) + c;
        if(dot(z, z) > 256.0f * 256.0f) {
            float colorVal = float(i) - log2(log2(dot(z, z)));
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
}
