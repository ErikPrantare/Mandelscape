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
https://github.com/ErikPrantare/Mandelscape/releases/tag/v0.1.0

# Controls
- <kbd>w</kbd><kbd>a</kbd><kbd>s</kbd><kbd>d</kbd> + mouse to walk and look around.

- <kbd>p</kbd> to pause/unpause.

- <kbd>j</kbd>/<kbd>k</kbd> to zoom in/out.

- <kbd>u</kbd>/<kbd>i</kbd> to change iteration count.

- <kbd>1</kbd>-<kbd>5</kbd> + <kbd>up</kbd>/<kbd>down</kbd> to change colors

- <kbd>x</kbd> to take screenshot.
    Will be put inside directory named screenshots.

- <kbd>h</kbd> to allow for a bit deeper zooms. Will increase lag drastically.

- <kbd>o</kbd> to toggle autozoom.

- <kbd>c</kbd> to toggle autowalk.

- <kbd>q</kbd> or <kbd>escape</kbd> to quit.

# Options
To use dvorak mode, start Mandelscape with `--dvorak` flag.
Controls can be found at the bottom of src/main.cpp.

# Additional info
Colors are calculated in shaders/shader.frag. This file can be edited to
use different coloring algorithms. shaders/orbit\_trap.frag contains a
coloring algorithm using orbit traps and the `time` variable to animate
it. Rename orbit\_trap.frag to shader.frag to see how it looks.

---

![](preview.jpg?raw=true "Title")
