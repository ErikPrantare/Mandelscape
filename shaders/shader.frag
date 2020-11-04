#version 140

precision highp float;

in vec2 position;
in float distance;
in float preCalculated;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 offset;
uniform int iterations;
uniform float colorFrequency;
uniform vec3 colorOffset;
uniform bool fastMode;

vec2
complexSquare(const in vec2 a)
{
    return vec2(a.x * a.x - a.y * a.y, 2.0 * a.x * a.y);
}

vec4
calculateColor(const in float val)
{
    float fogHardStart = 100.0;
    float fogHardEnd   = 150.0;
    float fog          = 1.0 - pow(0.98, distance);
    
    fog += clamp(
            (distance - fogHardStart) / (fogHardEnd - fogHardStart),
            0.0,
            1.0);
    
    fog = pow(fog, 2.0);

    if(val == -1.0) return vec4(fog, fog, fog, 1.0);

    vec3 colorVal = val * colorFrequency + colorOffset;
    return vec4(fog, fog, fog, 1.0)
            + (1.0 - fog) * texture(tex, vec2(0.0, val))
            * vec4(
                  0.5f * sin(colorVal) + 0.5f, 1.0f);
}

void
main()
{
    if(fastMode) {
        fragColor = calculateColor(preCalculated);
        return;
    }

    vec2 c = position + offset;

    // main cardioid check
    float q = pow(c.x - 0.25f, 2.0f) + c.y * c.y;
    if(q * (q + (c.x - 0.25f)) < 0.25f * c.y * c.y) {
        fragColor = calculateColor(-1.0);
        return;
    }

    // period-2 bulb check
    if((c.x + 1.0f) * (c.x + 1.0f) + c.y * c.y < 0.25f * 0.25f) {
        fragColor = calculateColor(-1.0);
        return;
    }

    vec2 z   = vec2(0.0, 0.0);

    for(int i = 0; i < iterations; ++i) {
        z = complexSquare(z) + c;
        if(dot(z, z) > 256.0f * 256.0f) {
            float val = float(i) - log2(log2(dot(z, z)));
            fragColor = calculateColor(val);
            return;
        }
    }

    fragColor = calculateColor(-1.0);
}
