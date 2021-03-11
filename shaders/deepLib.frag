/* Mandelscape
 * Copyright (C) 2020-2021 Erik Präntare
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

#define real vec2
#define complex vec4
#define re(z) (z).xy
#define im(z) (z).zw

real
add(const in real a, const in real b)
{
    vec2 s = twoSum(a.x, b.x);
    vec2 t = twoSum(a.y, b.y);
    s.y += t.x;
    s = quickTwoSum(s.x, s.y);
    s.y += t.y;
    return quickTwoSum(s.x, s.y);
}

complex
add(complex a, complex b)
{
    return complex(add(re(a), re(b)), add(im(a), im(b)));
}

real
mult(const in real a, const in real b)
{
    vec2 p = twoProd(a.x, b.x);
    p.y += a.x*b.y + a.y*b.x;
    return twoSum(p.x, p.y);
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

complex
square(const in complex a)
{
    return complex(
        add(square(re(a)), -square(im(a))),
        2.0 * mult(re(a), im(a)));
}

complex
makeComplex(const in vec2 hi, const in vec2 lo)
{
    return vec4(twoSum(hi.x, lo.x), twoSum(hi.y, lo.y));
}

float
toFloat(const in real a)
{
    return a.x;
}

real
toReal(const in float a)
{
    return real(a, 0.0);
}
