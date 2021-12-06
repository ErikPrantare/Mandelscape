# Mandelscape
Mandelscape is a landscape walking simulator based on the Mandelbrot fractal.
Walk around and explore the smallest of details by shrinking yourself.

## Build Guide (Linux)
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

## Binary Download (Windows)
https://github.com/ErikPrantare/Mandelscape/releases/tag/v1.0.0

## Controls
- Mouse: Look around.

- WASD: Walk around.

- SHIFT+WASD: Run around.

- P: Toggle pause(releases mouse).

- Left/Right mouse button: Shrink/grow.

- Q/Escape: Quit.

- CTRL+S/CTRL+O: Save/load preset.

- CTRL+L: Load scripts.

- J/K: Change iteration count.

- 1-4 + Up/Down: Change colors.

- 5 + Up/Down: Change terrain y scale.

- X: Render image. Will be put inside directory named screenshots.

- F: Toggle fast mode.

- H: Allow for deeper zooms. Will increase lag 
        (fast mode is recommended between rendering images).

- L: Toggle lighting effects.

- O: Toggle autozoom.

- C: Toggle autowalk.

## Options
To use dvorak mode, start Mandelscape with `--dvorak` flag.
Controls can be found in `src/keymaps.cpp`.

## Using different scripts
Loading different scripts is done with ctrl+l.
This opens up a file dialog, where you can select the script you want
to use.
Scripts can be identified by their file suffix `myAlgorithm<suffix>`,
with `<suffix>` being:

- `shape.lua`: Shape of the landscape.

- `value.frag`: In what way the landscape is colored.

- `color.frag`: Which colors are to be used when coloring the landscape.

Some algorithms can be found under `landscape-scripts`.

Detailed documentation on writing custom scripts is given in
`documentation/writing-custom-scripts.txt`.

---

![](preview.jpg?raw=true "Title")
