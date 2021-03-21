function logn(x, n)
    return math.log(x) / math.log(n);
end

function cmult(a, b)
    return {a[1]*b[1] - a[2]*b[2], a[1]*b[2] + a[2]*b[1]}
end

function cadd(a, b)
    return {a[1] + b[1], a[2] + b[2]}
end

function pointData (re, im, iters)
    dz = {0, 0}
    z = {0, 0}
    for i = 1,iters,1 do
        dz = cmult(cmult(z, z), dz)
        dz[2] = 3*dz[2];
        dz[1] = 3*dz[1] + 1;

        z = cmult(cmult(z, z), z);
        z = cadd(z, {re, im})
        
        if z[1]*z[1] + z[2]*z[2] > 256*256 then
            r = math.sqrt(z[1]*z[1] + z[2]*z[2])
            dr = math.sqrt(dz[1]*dz[1] + dz[2]*dz[2])
            de = 2.0 * r * math.log(r) / dr
            return {height = de, value = i - logn(logn(z[1]*z[1]+z[2]*z[2],3),3), inside = false}
        end
    end
        
    return {height = 0, value = 0, inside = true}
end
