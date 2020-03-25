#version 300 es

precision mediump float;

in vec4 color;
in vec3 position;
in float distance;
out vec4 fragColor;

uniform sampler2D tex;

#define pi 3.1415926535897932384626433832795

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

    vec2 c = position.xz;
    vec2 z = vec2(0.0, 0.0);

    // main cardioid check
    float q = pow(c.x - 0.25f, 2.0f) + c.y * c.y;
    if(q * (q + (c.x - 0.25f)) < 0.25f * c.y * c.y) {
        return;
    }

    // period-2 bulb check
    if((c.x + 1.0f) * (c.x + 1.0f) + c.y * c.y < 0.25f * 0.25f) {
        return;
    }

    for(int i = 0; i < 100; ++i) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        if(dot(z, z) > 256.0f * 256.0f) {
            float colorVal = float(i) - log2(log2(dot(z, z)));
            fragColor = fog*vec4(1.0, 1.0, 1.0, 1.0)
                        + (1.0-fog)
                              * texture(tex, vec2(0.0, colorVal))
                              * vec4(
                                  0.5f * sin(colorVal * 0.1f) + 0.5f,
                                  0.5f * sin(colorVal * 0.13f + 1.0f) + 0.5f,
                                  0.5f * sin(colorVal * 0.15f + 2.0f) + 0.5f,
                                  1.0f);
            return;
        }
    }
}
