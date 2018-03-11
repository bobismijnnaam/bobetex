![Bobetex logo](logo/logo_bg.png)

Bobetex
=======
###### An engine for hybrid development on native and on the web

_Under development_

## How to play
Arrow keys to walk around
Space to plant sugar
In the shop the following items can be bought from left to right:

- Movement speed
- Planting speed
- Planting shape
- Building blocks (not implemented)

Every item costs only one sugar, so you should have no problem farming enough to improve planting performance.

## Building

### Linux
Make sure you have SDL2 installed. Assumed path is `/usr/include/SDL2` for headers. libSDL2.a is assumed to be in path.

Then:

```bash
mkdir build && cd build
cmake ..
make
```

### Windows
```bash
# Make a build directory
cd build
cmake .. -G "Visual Studio 14 2015 Win64"
```

Then open the solution in your visual studio and build it!

It seems that the version you choose depends on the VS that you have available; I only tested with visual studi 2015 community edition. The most important part is that you type the Win64 part, since that SDL library (the x64 one) is hardcoded in the CMakeLists.txt. This is easily changed though.