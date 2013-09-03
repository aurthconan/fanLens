#include <SDL.h>

#include <fanFilm.h>
#include <fanVector.h>
#include <fanPixel.h>
#include <fanCamera.h>
#include <texture/film/SDLFilm.h>
#include <lens/OrthogonalLens.h>
#include <lens/PerspectiveLens.h>
#include <camera/PointScannerCamera.h>

#include <cstdlib>

using namespace fan;

int main(int argc, char** argv) {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }
    atexit(SDL_Quit);

    fanScene scene;
    /*
    for ( size_t i = 0; i < 200; i += 4 ) {
        for ( size_t j = 0; j < 200; j += 4 ) {
            for ( size_t k = 0; k < 200; k += 4 ) {
                scene.mVertices.push_back( fanVector3<float>( i, j, k ) );
            }
        }
    }
    */
    for ( size_t i = 0; i < 200; i += 10 ) {
        for ( size_t j = 0; j < 200; j += 10 ) {
            scene.mVertices.push_back( fanVector3<float>( i, j, 0 ) );
        }
    }
    for ( size_t i = 0; i < 200; i += 10 ) {
        for ( size_t j = 0; j < 200; j += 10 ) {
            scene.mVertices.push_back( fanVector3<float>( i, 0, j ) );
        }
    }
    for ( size_t i = 0; i < 200; i += 10 ) {
        for ( size_t j = 0; j < 200; j += 10 ) {
            scene.mVertices.push_back( fanVector3<float>( 0, i, j ) );
        }
    }
    for ( size_t i = 0; i < 201; i += 10 ) {
        for ( size_t j = 0; j < 200; j += 10 ) {
            scene.mVertices.push_back( fanVector3<float>( i, j, 200 ) );
        }
    }
    for ( size_t i = 0; i < 200; i += 10 ) {
        for ( size_t j = 0; j < 200; j += 10 ) {
            scene.mVertices.push_back( fanVector3<float>( i, 200, j ) );
        }
    }
    for ( size_t i = 0; i < 200; i += 10 ) {
        for ( size_t j = 0; j < 200; j += 10 ) {
            scene.mVertices.push_back( fanVector3<float>( 200, i, j ) );
        }
    }


    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    /*
    OrthogonalLens lens( fanVector3<float>(-1, -1, -1),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 1, 0) );
                         */
    PerspectiveLens lens( fanVector3<float>(0, 0, -1),
                          fanVector3<float>(0, 0, 0),
                          fanVector3<float>(0, 1, 0),
                          3000 );


    PointScannerCamera camera;

    bool done = false;
    while ( !done ) {
        camera.takePicture( scene, sdl, lens );
        sdl.develope();

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if ( event.type == SDL_QUIT ) {
                done = true;
            }
            switch( event.type ) {
                case SDL_QUIT:
                    done = true;
                break;
                case SDL_KEYDOWN:
                {
                    fanVector<float, 3> move;
                    switch( event.key.keysym.sym ) {
                        case SDLK_w: move[1] += 1; break;
                        case SDLK_s: move[1] -= 1; break;
                        case SDLK_a: move[0] -= 1; break;
                        case SDLK_d: move[0] += 1; break;
                    }
                    lens.move( move );
                }
                break;
            }
        }
    }
}

