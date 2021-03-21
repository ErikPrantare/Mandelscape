function cmult(a, b)
    return {a[1]*b[1] - a[2]*b[2], a[1]*b[2] + a[2]*b[1]}
end

function csquare(a)
    return {a[1]*a[1] - a[2]*a[2], 2*a[1]*a[2]}
end

function cadd(a, b)
    return {a[1] + b[1], a[2] + b[2]}
end

function pointData (re, im, iters)
    dz = {0, 0}
    z = {0, 0}
    c = {re, im}
    for i = 1,iters,1 do
        dz = cmult(z, dz)
        dz[2] = 2*dz[2];
        dz[1] = 2*dz[1] + 1;

        z = cadd(csquare(z), c);
        
        if z[1]*z[1] + z[2]*z[2] > 256*256 then
            r = math.sqrt(z[1]*z[1] + z[2]*z[2])
            dr = math.sqrt(dz[1]*dz[1] + dz[2]*dz[2])
            de = 2.0 * r * math.log(r) / dr

            return {height = de, value = i, inside = false}
        end
    end
        
    return {height = 0, value = 0, inside = true}
end
