#include <SDL.h>

#include <fanFilm.h>
#include <fanVector.h>
#include <fanPixel.h>
#include <fanCamera.h>
#include <fanTriangleMesh.h>
#include <fanBufferObject.h>
#include <texture/film/SDLFilm.h>
#include <fanLensMovement.h>
#include <lens/OrthogonalLens.h>
#include <lens/PerspectiveLens.h>
#include <camera/PointScannerCamera.h>
#include <camera/WireframeCamera.h>
#include <camera/DepthCamera.h>
#include <objects/TriangleMeshObject.h>

#include <boost/shared_ptr.hpp>

#include <cstdlib>

#include <iostream>

#include "QuadTree.h"

using namespace fan;
using namespace std;


int main() {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }

    fanScene scene;

    QuadTree tree( fanVector3<float>( 0, 0, 0 ),
                   fanVector3<float>( 1024, 0, 512 ),
                   fanVector3<float>( 0, 1024, 512 ),
                   fanVector3<float>( 1024, 1024, 1024 ) );

    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    fanVector3<float> pos(0, 0, 1000);
    fanVector3<float> lookAt(500, 500, 0);
    fanVector3<float> lookAxis = lookAt - pos;

    OrthogonalLens OrthoLens( pos,
                         lookAt,
                         fanVector3<float>(1, 1, 1),
                         fanVector3<float>(1600, 1200, 5000) );
    PerspectiveLens PerspLens( fanVector3<float>(0, 0, 1000),
                         fanVector3<float>(500, 500, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(1600, 1200, 5000),
                         3000 );

    fanCamera* currentCamera = NULL;
    fanLens* currentLens = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;
    DepthCamera depthCamera;

    bool done = false;

    boost::shared_ptr<TriangleMeshObject> terrain(
            new TriangleMeshObject( fanMatrix<float, 4, 4>{1, 0, 0, 0,
                                                           0, 1, 0, 0,
                                                           0, 0, 1, 0,
                                                           0, 0, 0, 1, } ) );

    currentCamera = &depthCamera;
    currentLens = &PerspLens;
    Uint32 time;
    scene.mTriangleMeshObjects.push_back( terrain );
    size_t level = 0;
    while ( !done ) {
        time = SDL_GetTicks();
        terrain->mMeshes.clear();
        tree.generateTerrian( *terrain, level );
        currentCamera->takePicture( scene, sdl, *currentLens );
        std::cout << SDL_GetTicks() - time << " ms in takePicture" << std::endl;
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
                        case SDLK_w: Pitch(M_PI/360.0f, OrthoLens); Pitch(M_PI/360.0f, PerspLens); break;
                        case SDLK_s: Pitch(-M_PI/360.0f, OrthoLens); Pitch(-M_PI/360.0f, PerspLens); break;
                        case SDLK_a: Yaw(M_PI/360.0f, OrthoLens); Yaw(M_PI/360.0f, PerspLens); break;
                        case SDLK_d: Yaw(-M_PI/360.0f, OrthoLens); Yaw(-M_PI/360.0f, PerspLens); break;
                        case SDLK_q: Roll(M_PI/360.0f, OrthoLens); Roll(M_PI/360.0f, PerspLens); break;
                        case SDLK_e: Roll(-M_PI/360.0f, OrthoLens); Roll(-M_PI/360.0f, PerspLens); break;
                        case SDLK_u: PitchAroundFocus(M_PI/360.0f, OrthoLens); PitchAroundFocus(M_PI/360.0f, PerspLens); break;
                        case SDLK_j: PitchAroundFocus(-M_PI/360.0f, OrthoLens); PitchAroundFocus(-M_PI/360.0f, PerspLens); break;
                        case SDLK_h: YawAroundFocus(M_PI/360.0f, OrthoLens); YawAroundFocus(M_PI/360.0f, PerspLens); break;
                        case SDLK_k: YawAroundFocus(-M_PI/360.0f, OrthoLens); YawAroundFocus(-M_PI/360.0f, PerspLens); break;
                        case SDLK_y: RollAroundFocus(M_PI/360.0f, OrthoLens); RollAroundFocus(M_PI/360.0f, PerspLens); break;
                        case SDLK_i: RollAroundFocus(-M_PI/360.0f, OrthoLens); RollAroundFocus(-M_PI/360.0f, PerspLens); break;
                        case SDLK_ESCAPE: done = true; break;
                        case SDLK_1: currentCamera = &pointCamera; break;
                        case SDLK_2: currentCamera = &wireframeCamera; break;
                        case SDLK_3: currentCamera = &depthCamera; break;
                        case SDLK_o: currentLens = &OrthoLens; break;
                        // case SDLK_p: currentLens = &PerspLens; break;
                        case SDLK_n: ++level; break;
                        case SDLK_b: if ( level != 0 ) --level; break;
                    }
                }
                break;
            }
        }
    }

    atexit(SDL_Quit);
}

