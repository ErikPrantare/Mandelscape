# Build Guide (Linux)
To build, run:
```
git clone https://github.com/ErikPrantare/MandelLandscape.git
cd MandelLandscape
cmake . -Bbuild
cmake --build ./build/
mv ./build/src/MandelLandscape .
```

If everything built correctly, the program can now be run from `./MandelLandscape`

# Binary Download (Windows)
A binary for windows is available through this torrent link:

magnet:?xt=urn:btih:2f841aba7d86c77499ce7428f49ee78542341cee&dn=MandelLandscape-0.0.1.zip&tr=udp%3a%2f%2ftracker.opentrackr.org%3a1337%2fannounce

# Controls
- <kbd>w</kbd><kbd>a</kbd><kbd>s</kbd><kbd>d</kbd> + mouse to walk and look around.

- <kbd>j</kbd>/<kbd>k</kbd> to zoom in/out.

- <kbd>u</kbd>/<kbd>i</kbd> to change iteration count.

- <kbd>h</kbd> to allow for a bit deeper zooms. Will increase lag drastically.

- <kbd>o</kbd> to toggle autozoom. Do not enable while standing on the black area.

- <kbd>q</kbd> to quit.

![](preview.jpeg?raw=true "Title")
