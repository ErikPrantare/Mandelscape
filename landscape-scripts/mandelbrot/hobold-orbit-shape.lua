--[===========================================================================[
  [ Mandelscape
  [ Copyright (C) 2021 Erik Präntare
  [ 
  [ This program is free software: you can redistribute it and/or modify
  [ it under the terms of the GNU Affero General Public License as published by
  [ the Free Software Foundation, either version 3 of the License, or
  [ (at your option) any later version.
  [ 
  [ This program is distributed in the hope that it will be useful,
  [ but WITHOUT ANY WARRANTY; without even the implied warranty of
  [ MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  [ GNU Affero General Public License for more details.
  [ 
  [ You should have received a copy of the GNU Affero General Public License
  [ along with this program.  If not, see <http://www.gnu.org/licenses/>.
--]===========================================================================]

function cmult(a, b)
    return {a[1]*b[1] - a[2]*b[2], a[1]*b[2] + a[2]*b[1]}
end

function csquare(a)
    return {a[1]*a[1] - a[2]*a[2], 2*a[1]*a[2]}
end

function cadd(a, b)
    return {a[1] + b[1], a[2] + b[2]}
end

function pointData(re, im, iters)
    dz = {0, 0}
    z = {0, 0}
    c = {re, im}
    minDist = 1e20
    nextMinDist = 1e20
    minIter = 0
    for i = 1,iters,1 do
        dz = cmult(z, dz)
        dz[2] = 2*dz[2];
        dz[1] = 2*dz[1] + 1;

        z = cadd(csquare(z), c);

        dist = z[1]*z[1] + z[2]*z[2]
        nextMinDist = dist < nextMinDist and dist or nextMinDist

        newClosest = dist < minDist
        nextMinDist = newClosest and minDist or nextMinDist
        minDist = newClosest and dist or minDist

        minIter = newClosest and i or minIter
        
        if dist > 256*256 then
            r = math.sqrt(z[1]*z[1] + z[2]*z[2])
            dr = math.sqrt(dz[1]*dz[1] + dz[2]*dz[2])
            de = 2.0 * r * math.log(r) / dr

            return {
                height = de*(1.0-minDist/nextMinDist),
                value = minIter,
                inside = false}
        end
    end
        
    return {height = 0, value = 0, inside = true}
end
