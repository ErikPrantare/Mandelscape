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

#define real float
#define complex vec2
#define re(z) (z).x
#define im(z) (z).y

real
add(const in real a, const in real b)
{
    return a + b;
}

complex
add(const in complex a, const in complex b)
{
    return a + b;
}

real
mult(const in real a, const in real b)
{
    return a * b;
}

complex
mult(const in complex a, const in complex b)
{
    return complex(re(a)*re(b) - im(a)*im(b), re(a)*im(b) + im(a)*re(b));
}

real
square(const in real a)
{
    return a * a;
}

complex
square(const in complex a)
{
    return vec2(re(a)*re(a) - im(a)*im(a), 2.0*re(a)*im(a));
}

complex
makeComplex(const in vec2 hi, const in vec2 lo)
{
    return complex(hi + lo);
}

float
toFloat(const in real a)
{
    return a;
}

real
toReal(const in float a)
{
    return a;
}
