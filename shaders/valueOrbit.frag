vec2
complexSquare(const in vec2 a)
{
    return vec2(a.x * a.x - a.y * a.y, 2.0 * a.x * a.y);
}

float
value(vec2 posHigh, vec2 posLow)
{
    vec2 c = posHigh + posLow;

    // main cardioid check
    float q = pow(c.x - 0.25f, 2.0f) + c.y * c.y;
    if(q * (q + (c.x - 0.25f)) < 0.25f * c.y * c.y) {
        return -1.0;
    }

    // period-2 bulb check
    if((c.x + 1.0f) * (c.x + 1.0f) + c.y * c.y < 0.25f * 0.25f) {
        return -1.0;
    }

    vec2 z   = vec2(0.0, 0.0);
    float dist = 1e10;

    vec2 point = 3.0*vec2(sin(0.0384*time)*sin(0.1*time), cos(0.1*time));
    float radius = 1.0 + sin(time*0.286);

    for(int i = 0; i < iterations; ++i) {
        z = complexSquare(z) + c;
        dist = min(dist, abs(dot(z - point, z - point) - radius));
        if(dot(z, z) > 256.0f * 256.0f) {
            return dist + float(i) - log2(log2(dot(z, z)));
        }
    }

    return -1.0;
}
