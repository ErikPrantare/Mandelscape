vec4
addFog(const in vec4 color)
{
    float fogHardStart = 100.0;
    float fogHardEnd   = 150.0;
    float fog          = 1.0 - pow(0.98, distance);
    
    fog += clamp(
            (distance - fogHardStart) / (fogHardEnd - fogHardStart),
            0.0,
            1.0);
    
    fog = pow(fog, 2.0);

    return vec4(fog, fog, fog, 1.0) + (1.0 - fog) * color;
}

void
main()
{
    if(fastMode) {
        fragColor =
            addFog(
                inside * color(preCalculated, true)
                + (1.0 - inside) * color(preCalculated, false));
        return;
    }

    bool inside;
    float val = value(makeComplex(position, offset), inside);
    fragColor = addFog(color(val, inside));
}
