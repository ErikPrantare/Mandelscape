/* Mandelscape
 * Copyright (C) 2021 Erik Präntare
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

#line 18 0

PointInfo
value(const in complex c)
{
    complex z = complex(0.0);

    for(int i = 0; i < iterations; ++i) {
        z = mult(mult(z, z), z);
        z = add(z, c);

        float distSquare = toFloat(add(square(re(z)), square(im(z))));
        if(distSquare > 256.0*256.0) {
            PointInfo p;
            p.inside = false;
            p.value = float(i) - log(log2(distSquare))/log(3.0);
            return p;
        }
    }

    PointInfo p;
    p.inside = true;
    return p;
}
