#define real float
#define complex vec2
#define re(z) (z).x
#define im(z) (z).y

vec2
csquare(const in vec2 a)
{
    return vec2(a.x * a.x - a.y * a.y, 2.0 * a.x * a.y);
}

complex
add(const in complex a, const in complex b)
{
    return a + b;
}

float
toFloat(const in real a)
{
    return a;
}

real
add(const in real a, const in real b)
{
    return a + b;
}

real
mult(const in real a, const in real b)
{
    return a * b;
}

complex
makeComplex(const in vec2 hi, const in vec2 lo)
{
    return complex(hi + lo);
}
