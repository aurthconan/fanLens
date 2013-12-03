fanLens
=======

A toy software renderer. For fun and my interest in Computer Graphics.
Currently it support point mode, wireframe mode, flat shading, Gouraud shading, Phong shading.
It also has a standard shadow map implementation.

Performance is not good, future optimization is needed.

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

The model_viewer needs assimp (http://assimp.sourceforge.net/) to load model file. The following path need to be added
 - ASSIMP_DIR= # assimp include path
 - ASSIMP_LIB= # libassimp.so path
