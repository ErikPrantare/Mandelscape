# Mandelscape
Mandelscape is a landscape walking simulator based on the Mandelbrot fractal.
Walk around and explore the smallest of details by shrinking yourself.

# Build Guide (Linux)
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
- wasd + mouse to walk and look around.

- p to pause/unpause.

- left/right mouse button to shrink/grow.

- j/k to change iteration count.

- 1-4 + up/down to change colors.

- 5 + up/down to terrain y scale.

- x to render image. Will be put inside directory named screenshots.

- f to toggle fast mode.

- h to allow for deeper zooms. Will increase lag 
        (fast mode is recommended between rendering images).

- o to toggle autozoom.

- c to toggle autowalk.

- q or escape to quit.

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
