float
value(const in complex c)
{
    float ci = toFloat(im(c));
    float cr = toFloat(re(c));

    // main cardioid check
    float q = pow(cr - 0.25f, 2.0) + ci*ci;
    if(q * (q + (cr - 0.25f)) < 0.25f * ci * ci) {
        return -1.0;
    }

    // period-2 bulb check
    if((ci + 1.0f) * (ci + 1.0f) + ci * ci < 0.25f * 0.25f) {
        return -1.0;
    }

    complex z   = complex(0.0);
    float minDist = 1e10;

    complex point = 
        3.0*complex(toReal(sin(0.0384*time)*sin(0.1*time)), toReal(cos(0.1*time)));

    for(int i = 0; i < iterations; ++i) {
        z = add(square(z), c);
        
        complex cdist = add(z, -point);
        minDist = min(minDist,toFloat(re(cdist)*re(cdist) + im(cdist)*im(cdist)));
        float dist = 
            toFloat(add(mult(re(cdist), re(cdist)),
                        mult(im(cdist), im(cdist))));
        if(dist > 256.0f * 256.0f) {
            return minDist + float(i) - log2(log2(dist));
        }
    }

    return -1.0;
}
