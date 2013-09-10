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

#include <cstdlib>

using namespace fan;

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
            fanTriangle t1( &scene.mVertices[p0Index], &scene.mVertices[p1Index],
                            &scene.mVertices[p2Index] );
            fanTriangle t2( &scene.mVertices[p1Index], &scene.mVertices[p2Index],
                            &scene.mVertices[p3Index] );
            scene.mTriangles.push_back(t1);
            scene.mTriangles.push_back(t2);
        }
    }
}

int main() {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }
    atexit(SDL_Quit);

    fanScene scene;

    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, j, 0 ) );
        }
    }
    addTriangle( scene, 0 );
    size_t vertexNum = scene.mVertices.size();
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, 0, j ) );
        }
    }
    /*
    addTriangle( scene, vertexNum );
    vertexNum = scene.mVertices.size();
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( 0, i, j ) );
        }
    }
    //addTriangle( scene, vertexNum );
    vertexNum = scene.mVertices.size();
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, j, 200 ) );
        }
    }
    //addTriangle( scene, vertexNum );
    vertexNum = scene.mVertices.size();
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( i, 200, j ) );
        }
    }
    //addTriangle( scene, vertexNum );
    vertexNum = scene.mVertices.size();
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            scene.mVertices.push_back( fanVector3<float>( 200, i, j ) );
        }
    }
    //addTriangle( scene, vertexNum );
    */

    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    OrthogonalLens lens( fanVector3<float>(1, 1, 1),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1) );
    /*
    PerspectiveLens lens( fanVector3<float>(1, 1, 1),
                          fanVector3<float>(0, 0, 0),
                          fanVector3<float>(0, 0, 1),
                          3000 );
                          */

    fanCamera* currentCamera = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;

    bool done = false;

    currentCamera = &pointCamera;
    while ( !done ) {
        currentCamera->takePicture( scene, sdl, lens );
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
                        case SDLK_a: move[0] += 1; break;
                        case SDLK_d: move[0] -= 1; break;
                        case SDLK_ESCAPE: done = true; break;
                        case SDLK_1: currentCamera = &pointCamera; break;
                        case SDLK_2: currentCamera = &wireframeCamera; break;
                    }
                    lens.move( move );
                }
                break;
            }
        }
    }
}

