#include <SDL.h>

#include <fanFilm.h>
#include <fanVector.h>
#include <fanPixel.h>
#include <fanCamera.h>
#include <texture/film/SDLFilm.h>
#include <lens/OrthogonalLens.h>
#include <lens/PerspectiveLens.h>
#include <camera/PointScannerCamera.h>
#include <camera/WireframeCamera.h>
#include <utils/StopWatch.h>

#include <cstdlib>

#include <iostream>

using namespace fan;
using namespace std;

void addTriangle( fanScene& scene, size_t startIndex ) {
    for( size_t i = 0; i < 4; ++i ) {
        for( size_t j = 0; j < 4; ++j ) {
            /*
             * p2--p3
             *  |  |
             * p0--p1
             */
            size_t p0Index = startIndex + i * 5 + j;
            size_t p1Index = startIndex + i * 5 + j + 1;
            size_t p2Index = p0Index + 5;
            size_t p3Index = p1Index + 5;
            // every face has two triangle
            fanTriangle t1( &scene.mVertices[p0Index], &scene.mVertices[p2Index],
                            &scene.mVertices[p1Index] );
            fanTriangle t2( &scene.mVertices[p1Index], &scene.mVertices[p2Index],
                            &scene.mVertices[p3Index] );
            scene.mTriangles.push_back(t1);
            scene.mTriangles.push_back(t2);
        }
    }
}

void dumpTriangles( fanScene& scene ) {
    for ( size_t i = 0, max = scene.mTriangles.size();
            i < max; ++i ) {
        fanVector3<float>* a = scene.mTriangles[i].a;
        fanVector3<float>* b = scene.mTriangles[i].b;
        fanVector3<float>* c = scene.mTriangles[i].c;

        cout << " index " << i << endl;
        cout << "a x:" << (*a)[0] << " y:" << (*a)[1]
                << " z:" << (*a)[2] << endl;
        cout << "b x:" << (*b)[0] << " y:" << (*b)[1]
                << " z:" << (*b)[2] << endl;
        cout << "c x:" << (*c)[0] << " y:" << (*c)[1]
                << " z:" << (*c)[2] << endl;
    }
}

int main() {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }

    fanScene scene;

    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, j, 0 ) );
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, 0, j ) );
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( 0, i, j ) );
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, j, 200 ) );
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, 200, j ) );
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( 200, i, j ) );
        }
    }
    addTriangle( scene, 0 );
    addTriangle( scene, 25 );
    addTriangle( scene, 50 );
    addTriangle( scene, 75 );
    addTriangle( scene, 100 );
    addTriangle( scene, 125 );

    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    OrthogonalLens OrthoLens( fanVector3<float>(400, 400, 400),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(800, 600, 1000) );
    PerspectiveLens PerspLens( fanVector3<float>(400, 400, 400),
                          fanVector3<float>(0, 0, 0),
                          fanVector3<float>(0, 0, 1),
                          fanVector3<float>(800, 600, 1000),
                          3000 );

    fanCamera* currentCamera = NULL;
    fanLens* currentLens = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;

    bool done = false;

    currentCamera = &pointCamera;
    currentLens = &PerspLens;

    StopWatch stopWatch;
    while ( !done ) {
        stopWatch.start();
        currentCamera->takePicture( scene, sdl, *currentLens );
        stopWatch.stop("Take Picture");
        sdl.develope();

        SDL_Event event;
        if (SDL_WaitEvent(&event)) {
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
                        case SDLK_ESCAPE: done = true; break;
                        case SDLK_1: currentCamera = &pointCamera; break;
                        case SDLK_2: currentCamera = &wireframeCamera; break;
                        case SDLK_o: currentLens = &OrthoLens; break;
                        case SDLK_p: currentLens = &PerspLens; break;
                    }
                    PerspLens.move( move );
                    OrthoLens.move( move );
                }
                break;
            }
        }
    }

    atexit(SDL_Quit);
}

