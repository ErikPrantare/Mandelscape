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

#line 18 1

float
value(const in complex c, out bool inside)
{
    float ci = toFloat(im(c));
    float cr = toFloat(re(c));

    // main cardioid check
    float q = pow(cr - 0.25f, 2.0) + ci*ci;
    if(q * (q + (cr - 0.25f)) < 0.25f * ci * ci) {
        inside = true;
        return -1.0;
    }

    // period-2 bulb check
    if((ci + 1.0f) * (ci + 1.0f) + ci * ci < 0.25f * 0.25f) {
        inside = true;
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
            inside = false;
            return float(i) - log2(log2(toFloat(add(realSquare, imagSquare))));
        }
    }

    inside = true;
    return -1.0;
}
