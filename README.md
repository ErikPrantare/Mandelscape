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
https://github.com/ErikPrantare/Mandelscape/releases/tag/v1.0.0

# Controls
- WASD + mouse: Walk and look around.

- P: Toggle pause(releases mouse).

- left/right mouse button: Shrink/grow.

- Q/escape: Quit.

- CTRL+S/CTRL+O: Save/load state.

- CTRL+L: Load different algorithms.

- J/K: Change iteration count.

- 1-4 + up/down: Change colors.

- 5 + up/down: Change terrain y scale.

- X: Render image. Will be put inside directory named screenshots.

- F: Toggle fast mode.

- H: Allow for deeper zooms. Will increase lag 
        (fast mode is recommended between rendering images).

- O: Toggle autozoom.

- C: Toggle autowalk.

# Options
To use dvorak mode, start Mandelscape with `--dvorak` flag.
Controls can be found at the bottom of src/main.cpp.

# Using different algorithms
Loading different algorithms is done with ctrl+l.
This opens up a file dialog, where you can select the algorithms you want
to use.
Algorithms can be identified by their file suffix `myAlgorithm<suffix>`,
with `<suffix>` being:

- `shape.lua`: Algorithm changing the terrain shape.

- `value.frag`: Algorithm changing the way the terrain is colored.

- `color.frag`: Algorithm changing what colors are used.

Some algorithms can be found under `presets`.

Detailed documentation on writing custom algorithms is given in
`documentation/writing-custom-algorithms.txt`.

---

![](preview.jpg?raw=true "Title")
