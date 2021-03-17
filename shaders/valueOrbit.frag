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

PointInfo
value(const in complex c)
{
    PointInfo p;
    p.value = -1.0;

    float ci = toFloat(im(c));
    float cr = toFloat(re(c));

    // main cardioid check
    float q = pow(cr - 0.25f, 2.0) + ci*ci;
    if(q * (q + (cr - 0.25f)) < 0.25f * ci * ci) {
        p.inside = true;
        return p;
    }

    // period-2 bulb check
    if((ci + 1.0f) * (ci + 1.0f) + ci * ci < 0.25f * 0.25f) {
        p.inside = true;
        return p;
    }

    complex z   = complex(0.0);
    float minDist = 1e10;

    complex point = 
        3.0*complex(
                toReal(sin(0.0384*time)*sin(0.1*time)),
                toReal(cos(0.1*time)));

    for(int i = 0; i < iterations; ++i) {
        z = add(square(z), c);
        
        complex cdist = add(z, -point);
        minDist = min(
                minDist,
                toFloat(re(cdist)*re(cdist) + im(cdist)*im(cdist)));

        float dist = 
            toFloat(add(mult(re(cdist), re(cdist)),
                        mult(im(cdist), im(cdist))));
        if(dist > 256.0f * 256.0f) {
            p.inside = false;
            p.value = minDist + float(i) - log2(log2(dist));
            return p;
        }
    }

    p.inside = true;
    return p;
}
