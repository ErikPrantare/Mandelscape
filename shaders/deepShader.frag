#version 300 es

precision highp float;

in vec2 position;
in float distance;
out vec4 fragColor;

uniform sampler2D tex;
uniform vec2 offset;
uniform int iterations;

// HACK: Use variable to suppress floating point optimizations
float ONE = 1.0;

//a > b
vec2
quickTwoSum(float a, float b) 
{
    float sum = a + b;
    float error = b - (sum*ONE - a);
    return vec2(sum, error);
}

vec2
twoSum(float a, float b)
{
    float sum = a + b;
    float v = sum*ONE - a;
    float error = (a - (sum - v)) + (b - v);
    return vec2(sum, error);
}

vec2
split(float a)
{
    float t = (pow(2.0, 12.0) + 1.0) * a;
    float hi = t - (t - a);
    float lo = a - hi;
    return vec2(hi, lo);
}

vec2
twoProd(float a, float b)
{
    float prod = a * b;
    vec2 as = split(a);
    vec2 bs = split(b);
    float error = ((as.x*bs.x - prod) + as.x*bs.y + as.y*bs.x) + as.y*bs.y;
    return vec2(prod, error);
}

vec2
doubleSum(vec2 a, vec2 b)
{
    vec2 s = twoSum(a.x, b.x);
    vec2 t = twoSum(a.y, b.y);
    s.y += t.x;
    s = quickTwoSum(s.x, s.y);
    s.y += t.y;
    return quickTwoSum(s.x, s.y);
}

vec2
doubleProd(vec2 a, vec2 b)
{
    vec2 p = twoProd(a.x, b.x);
    p.y += a.x*b.y + a.y*b.x;
    return twoSum(p.x, p.y);
}

vec4
doubleCompSum(vec4 a, vec4 b)
{
    return vec4(doubleSum(a.xy, b.xy), doubleSum(a.zw, b.zw));
}

void
main()
{
    ONE = float(iterations/iterations);
    float fogHardStart = 100.0;
    float fogHardEnd   = 150.0;
    float fog          = 1.0 - pow(0.98, distance);
    fog +=
            clamp((distance - fogHardStart) / (fogHardEnd - fogHardStart),
                  0.0,
                  1.0);
    fog       = pow(fog, 2.0);
    fragColor = vec4(fog, fog, fog, 1.0);

    vec2 c_ = position + offset;

    // main cardioid check
    float q = pow(c_.x - 0.25f, 2.0f) + c_.y * c_.y;
    if(q * (q + (c_.x - 0.25f)) < 0.25f * c_.y * c_.y) {
        return;
    }

    // period-2 bulb check
    if((c_.x + 1.0f) * (c_.x + 1.0f) + c_.y * c_.y < 0.25f * 0.25f) {
        return;
    }

    // a.xy: real, a.zw: imag
    vec4 c  = vec4(twoSum(offset.x, position.x),
                    twoSum(offset.y, position.y));
    vec4 z   = vec4(0.0, 0.0, 0.0, 0.0);
    vec2 realSquare = vec2(0.0, 0.0);
    vec2 imagSquare = vec2(0.0, 0.0);

    for(int i = 0; i < iterations; ++i) {
        z.zw = doubleProd(z.xy, z.zw); 
        z.zw += z.zw;
        z.xy = doubleSum(realSquare, -imagSquare);
        z = doubleCompSum(z, c);
        realSquare = doubleProd(z.xy, z.xy);
        imagSquare = doubleProd(z.zw, z.zw);
        if(realSquare.x + imagSquare.x > 256.0f * 256.0f) {
            float colorVal = float(i) - log2(log2(realSquare.x + imagSquare.x));
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
