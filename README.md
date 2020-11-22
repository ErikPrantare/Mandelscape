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
- wasd + mouse to walk and look around.

- p to pause/unpause.

- left/right mouse button to zoom in/out.

- j/k to change iteration count.

- 1-4 + up/down to change colors.

- 5 + up/down to terrain y scale.

- x to render image. Will be put inside directory named screenshots.

- h to allow for deeper zooms. Will increase lag 
        (fast mode is recommended between rendering images).

- o to toggle autozoom.

- c to toggle autowalk.

- q or escape to quit.

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
