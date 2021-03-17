# Mandelscape
Mandelscape is a landscape walking simulator based on the Mandelbrot fractal.
Walk around and explore the smallest of details by shrinking yourself.

# Build Guide (Linux)
Mandelscape depends on GTK3. To install it on Debian:
```
sudo apt install libgtk-3-dev
```

To compile Mandelscape:
```
git clone https://github.com/ErikPrantare/Mandelscape.git
cd Mandelscape
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release
cmake --build build --target Mandelscape
mv build/src/Mandelscape .
```
If everything built correctly, the program can now be run with `./Mandelscape`

# Binary Download (Windows)
https://github.com/ErikPrantare/Mandelscape/releases/tag/v0.2.0

# Controls
- wasd + mouse: Walk and look around.

- p: toggle Pause(releases mouse).

- left/right mouse button: Shrink/grow.

- q/escape: Quit.

- ctrl+s/ctrl+o: Save/load state.

- j/k: Change iteration count.

- 1-4 + up/down: Change colors.

- 5 + up/down: Change terrain y scale.

- x: Render image. Will be put inside directory named screenshots.

- f: Toggle fast mode.

- h: Allow for deeper zooms. Will increase lag 
        (fast mode is recommended between rendering images).

- o: Toggle autozoom.

- c: Toggle autowalk.

# Options
To use dvorak mode, start Mandelscape with `--dvorak` flag.
Controls can be found at the bottom of src/main.cpp.

# Modifying the coloring algorithm
Detailed documentation on writing custom coloring algorithms is given in
documentation/writing-custom-algorithms.txt.

Try renaming
`shaders/valueOrbit.frag`.
to
`shaders/value.frag`
to see what is posible to achieve with different algorithms.

---

![](preview.jpg?raw=true "Title")
