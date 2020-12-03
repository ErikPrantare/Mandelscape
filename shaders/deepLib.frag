//suppress floating point optimizations for addition
float
plusIeeeCompliant(float a, float b)
{
    return mix(a, a+b, b != 0);
}

// prerequisite: a > b
vec2
quickTwoSum(float a, float b) 
{
    float sum = plusIeeeCompliant(a, b);
    float error = b - (sum - a);
    return vec2(sum, error);
}

vec2
twoSum(float a, float b)
{
    float sum = plusIeeeCompliant(a, b);
    float v = sum - a;
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
add(vec2 a, vec2 b)
{
    vec2 s = twoSum(a.x, b.x);
    vec2 t = twoSum(a.y, b.y);
    s.y += t.x;
    s = quickTwoSum(s.x, s.y);
    s.y += t.y;
    return quickTwoSum(s.x, s.y);
}

vec2
mult(vec2 a, vec2 b)
{
    vec2 p = twoProd(a.x, b.x);
    p.y += a.x*b.y + a.y*b.x;
    return twoSum(p.x, p.y);
}

#define real vec2
#define complex vec4
#define re(z) (z).xy
#define im(z) (z).zw

complex
add(complex a, complex b)
{
    return complex(add(re(a), re(b)), add(im(a), im(b)));
}

real
add(const in real a, const in float b)
{
    return add(a, real(b, 0.0));
}

complex
mult(const in complex a, const in complex b)
{
    return complex(
        add(mult(re(a), re(b)), -mult(im(a), im(b))),
        add(mult(im(a), re(b)), mult(re(a), im(b))));
}

real
square(const in real a)
{
    return mult(a, a);
}

float
toFloat(const in real a)
{
    return a.x;
}

complex
makeComplex(const in vec2 hi, const in vec2 lo)
{
    return vec4(twoSum(hi.x, lo.x), twoSum(hi.y, lo.y));
}
