#line 1 0

vec4
color(const in float val, bool inside)
{
    if(inside) return vec4(0.0, 0.0, 0.0, 1.0);

    vec3 colorVal = val * colorFrequency + colorOffset;

    //val + 0.1 avoids weird color glitches in when val is integer
    vec4 color = texture(tex, vec2(0.0, val + 0.1))
            * vec4(0.5f * sin(colorVal) + 0.5f, 1.0f);

    return color;
}
