#version 330

precision highp float;

in vec2 position;
in float distance;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 offset;
uniform int iterations;
uniform float time;
uniform float colorFrequency;
uniform vec3 colorOffset;

vec2
complexSquare(const in vec2 a)
{
    return vec2(a.x * a.x - a.y * a.y, 2.0 * a.x * a.y);
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
    float dist = 1e10;

    vec2 point = 3.0*vec2(sin(0.0384*time)*sin(0.1*time), cos(0.1*time));
    float radius = 1.0 + sin(time*0.286);

    for(int i = 0; i < iterations; ++i) {
        z = complexSquare(z) + c;
        dist = min(dist, abs(dot(z - point, z - point) - radius));
        if(dot(z, z) > 256.0f * 256.0f) {
            float val = dist + float(i) - log2(log2(dot(z, z)));
            vec3 colorVal = val * colorFrequency + colorOffset;
            fragColor =
                    fog * vec4(1.0, 1.0, 1.0, 1.0)
                    + (1.0 - fog) * texture(tex, vec2(0.0, 0.5*val))
                    * vec4(0.5f * sin(colorVal) + 0.5f, 1.0f);
            return;
        }
    }
}

