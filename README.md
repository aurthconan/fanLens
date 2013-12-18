fanLens
=======

A toy software renderer. For fun and my interest in Computer Graphics.
Currently it support point mode, wireframe mode, flat shading, Gouraud shading, Phong shading.
It also has a standard shadow map implementation.

Performance is not good, future optimization is needed.

Examples
------
![Light over the teapot](https://raw.github.com/aurthconan/just_showcase/master/rainbow_light_over_teapot.png)

A point light emits rainbow like lights over a standard teapot.

How to build
-------
Sadly I did not have time to do a better build script yet (maybe GNU automake or cmake in the future).

I'll list dependency here:
 - FreeImageFilm needs libfreeimage
 - SDLFilm needs SDL 2.0
 - boost::shared_ptr

If yout need to build unittest and sample, you need to add path to the following variable in Makefile
 - GTEST_DIR= # google test framework folder
 - SDL_DIR= # SDL 2.0 include path
 - SDL_LIB= # libSDL2-2.0 path

Samples use the following library to load corresponding model file:
 - ply: assimp(http://assimp.sourceforge.net/) ( ps: I plan to replace this in the future )
 - 3ds: lib3ds(http://code.google.com/p/lib3ds/)

If you need to build model_viewer example, make sure the following variable is modified in Makefile
 - ASSIMP_DIR= # assimp include path
 - ASSIMP_LIB= # libassimp.so path
 - LIB3DS_DIR= # lib3ds.h location
 - LIB3DS_DIR= # lib3ds.so location
