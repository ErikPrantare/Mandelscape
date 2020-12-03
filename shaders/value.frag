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
    real realSquare = real(0.0);
    real imagSquare = real(0.0);

    for(int i = 0; i < iterations; ++i) {
        im(z) = mult(re(z), im(z)); 
        im(z) += im(z);
        re(z) = add(realSquare, -imagSquare);
        z = add(z, c);

        realSquare = mult(re(z), re(z));
        imagSquare = mult(im(z), im(z));

        if(toFloat(realSquare) + toFloat(imagSquare) > 256.0f * 256.0f) {
            return float(i) - log2(log2(toFloat(add(realSquare, imagSquare))));
        }
    }

    return -1.0;
}
