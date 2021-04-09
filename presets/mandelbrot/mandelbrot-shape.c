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

#include <stdbool.h>
#include <math.h>
#include <complex.h>

struct PointData {
    double height;
    double value;
    bool inside;
};

double
realSquare(double complex z)
{
  return pow(creal(z), 2) + pow(cimag(z), 2);
}

struct PointData
pointData(double real, double imag, int iterations)
{
    double complex dz = 0.0 + 0.0i;
    double complex z  = 0.0 + 0.0i;
    double complex c  = real + imag * I;

    for(int i = 0; i < iterations; i++) {
        dz = z * dz;
        dz = 2.0 * dz + 1.0;
        z  = cpow(z, 2.0) + c;

        if(realSquare(z) > pow(256.0, 2.0)) {
            double r  = sqrt(realSquare(z));
            double dr = sqrt(realSquare(dz));
            double de = 2.0 * r * log(r) / dr;

            struct PointData const pd = {de, (double)i, false};
            return pd;
        }
    }

    struct PointData const pd = {0.0, 0.0, true};
    return pd;
}

/*function pointData (re, im, iters)*/
/*dz = {0, 0}*/
/*z = {0, 0}*/
/*c = {re, im}*/
/*for i = 1,iters,1 do*/
/*dz = cmult(z, dz)*/
/*dz[2] = 2*dz[2];*/
/*dz[1] = 2*dz[1] + 1;*/

/*z = cadd(csquare(z), c);*/

/*if z[1]*z[1] + z[2]*z[2] > 256*256 then*/
/*r = math.sqrt(z[1]*z[1] + z[2]*z[2])*/
/*dr = math.sqrt(dz[1]*dz[1] + dz[2]*dz[2])*/
/*de = 2.0 * r * math.log(r) / dr*/

/*return {height = de, value = i, inside = false}*/
/*end*/
/*end*/

/*return {height = 0, value = 0, inside = true}*/
/*end*/
