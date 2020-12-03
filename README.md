# Mandelscape
Mandelscape is a landscape walking simulator based on Mandelbrot.
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
Colors are calculated in two steps. Changing the coloring can be achieved
by changing any of these steps.

1. Calculate the value of the point.
A simple example of this would be to use the number of iterations it took to
bail out as the value.
This value is calculated in `shaders/value.frag`
(or `shaders/valueDeep.frag` for deeper zooms).

2. Calculate the color.
This takes the value of the previous step, and maps it to a color.
This function is defined in `shaders/color.frag`.

An example of a different value function is given in
`shaders/valueOrbit.frag`.
Rename this to `shaders/value.frag`, and see how the coloring differs.

---

![](preview.jpg?raw=true "Title")
