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

#line 18 1

PointInfo
value(const in complex c)
{
    complex z   = complex(0.0);

    float minDist = 1e10;
    float nextMinDist = 1e10;
    int minIter = 0;

    for(int i = 0; i < iterations; ++i) {
        z = add(square(z), c);
        
        complex cdist = z;

        float dist = 
            toFloat(add(mult(re(cdist), re(cdist)),
                        mult(im(cdist), im(cdist))));

        nextMinDist = min(dist, nextMinDist);
        bool newClosest = dist < minDist;
        nextMinDist = newClosest ? minDist : nextMinDist;
        minDist = newClosest ? dist : minDist;

        minIter = newClosest ? i : minIter;

        if(dist > 256.0f * 256.0f) {
            PointInfo p;
            p.inside = false;
            p.value = float(minIter) - minDist/nextMinDist;
            return p;
        }
    }

    PointInfo p;
    p.inside = false;
    p.value = float(minIter) - minDist/nextMinDist;
    return p;
}
