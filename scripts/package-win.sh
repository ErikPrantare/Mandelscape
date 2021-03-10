#!/bin/sh

# clean previous build
rm -r build/ || true &&
cd extern/luajit-2.0/ &&
make clean &&
cd ../.. &&

cmake . -DBUILD_TESTING=OFF \
    -DCMAKE_CXX_COMPILER="/usr/bin/x86_64-w64-mingw32-g++-posix" \
    -DCMAKE_C_COMPILER="/usr/bin/x86_64-w64-mingw32-gcc" \
    -DCMAKE_SYSTEM_NAME=Windows \
    -Bbuild &&

cmake --build ./build/ &&

mv build/src/Mandelscape.exe . &&
mv extern/luajit-2.0/src/lua51.dll . &&
zip Mandelscape.zip \
    Mandelscape.exe LICENSE shaders/* textures/* lua51.dll \
    README.md documentation/* &&

# clean
rm -r build/ &&
cd extern/luajit-2.0/ &&
make clean &&
cd ../.. &&
rm Mandelscape.exe &&
rm lua51.dll &&

echo "Packaging into Mandelscape.zip done"
