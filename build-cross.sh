#!/bin/sh

rm -r build
cmake . -DBUILD_TESTING=OFF \
    -G Ninja \
    -DCMAKE_SYSROOT="/usr/i686-w64-mingw32" \
    -DCMAKE_CXX_COMPILER="/usr/bin/i686-w64-mingw32-g++" \
    -DCMAKE_C_COMPILER="/usr/bin/i686-w64-mingw32-gcc" \
    -DCMAKE_RC_COMPILER="/usr/bin/i686-w64-mingw32-windres" \
    -DCMAKE_CROSSCOMPILING=TRUE \
    -DCMAKE_SYSTEM_NAME=Windows -Bbuild &&

cmake --build ./build/ -- -j8
