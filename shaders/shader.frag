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

    vec2 z   = c;
    float dist1 = 1e10;
    float dist2 = 1e10;
    float dist3 = 1e10;
    vec2 point1 = 0.3*vec2(cos(0.26398*time), sin(0.3*time));
    vec2 point2 = 0.3*vec2(cos(0.86577*time), sin(0.7*time));
    vec2 point3 = 0.3*vec2(cos(0.684*time), sin(0.5*time));

    for(int i = 0; i < 200; ++i) {
        z = complexMultiplication(z, z) + c;
        dist1 = min(dist1, dot(z-point1,z-point1));
        dist2 = min(dist2, dot(z-point2,z-point2));
        dist3 = min(dist3, dot(z-point3,z-point3));
        if(dot(z, z) > 256.0f * 256.0f) {
            float colorVal = float(i) - log2(log2(dot(z, z)));
    fragColor = vec4(vec3(1.0, 1.0, 1.0) - pow(vec3(sqrt(dist1), sqrt(dist2), sqrt(dist3)), vec3(0.2, 0.2, 0.2)), 1.0);
            fragColor.x *= 0.5f*sin(colorVal * 0.13f + time) + 0.5f;
            fragColor *= vec4(1.0-fog, 1.0-fog, 1.0-fog, 1.0);
            return;
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

    fragColor = vec4(pow(vec3(sqrt(dist1), sqrt(dist2), sqrt(dist3)), vec3(0.2, 0.2, 0.2)), 1.0);
}
