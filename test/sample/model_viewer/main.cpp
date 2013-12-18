#include <SDL.h>

#include <fanFilm.h>
#include <fanVector.h>
#include <fanPixel.h>
#include <fanCamera.h>
#include <texture/film/SDLFilm.h>
#include <fanLensMovement.h>
#include <fanTriangleMesh.h>
#include <fanBufferObject.h>
#include <lens/OrthogonalLens.h>
#include <lens/PerspectiveLens.h>
#include <camera/PointScannerCamera.h>
#include <camera/WireframeCamera.h>
#include <camera/DepthCamera.h>
#include <filler/FlatShadingFiller.h>
#include <filler/GouraudShadingFiller.h>
#include <filler/PhongShadingFiller.h>
#include <camera/RasterisationCamera.h>
#include <lights/Ambient.h>
#include <lights/PointLight.h>
#include <lights/RainbowLight.h>
#include <lights/ShadowUsingZBufferLight.h>
#include <objects/TriangleMeshObject.h>

#include <cstdlib>
#include <cmath>

#include <vector>
#include <iostream>
#include <limits>
#include <cmath>

#include <boost/shared_ptr.hpp>

#include <LoadFile.h>

using namespace std;

using namespace fan;

float radius;
fanVector3<float> boundBoxMin;
fanVector3<float> boundBoxMax;

int main(int argc, char** argv) {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }
    atexit(SDL_Quit);

    fanScene scene;

    if ( argc < 2 ) {
        return 2;
    }

    boundBoxMin[0] = boundBoxMin[1] = boundBoxMin[2] = std::numeric_limits<float>::max();
    boundBoxMax[0] = boundBoxMax[1] = boundBoxMax[2] = std::numeric_limits<float>::min();

    if ( !loadFile( argv[1], scene, boundBoxMin, boundBoxMax ) ) {
        return 3;
    }

    float xBound = boundBoxMax[0] - boundBoxMin[0];
    float yBound = boundBoxMax[1] - boundBoxMin[1];
    float zBound = boundBoxMax[2] - boundBoxMin[2];
    fanVector3<float> center(boundBoxMin[0] + xBound/2,
                             boundBoxMin[1] + yBound/2,
                             boundBoxMin[2] + zBound/2);
    float diameter = sqrt( xBound*xBound + yBound*yBound + zBound*zBound );
    radius = diameter / 2.0f;

    //add lights
    // ambient
    boost::shared_ptr<fanLight> light(
            new Ambient( fanPixel( 255, 50, 50, 50 ) ) );
    scene.mLights.push_back( light );
    boost::shared_ptr<PointLight> point;
    boost::shared_ptr<ShadowUsingZBufferLight> shadow;

    // moving light
    point.reset( new RainbowLight( center + fanVector3<float>(-diameter, 0, diameter) ) );
    shadow.reset( new ShadowUsingZBufferLight( point,
             fanVector3<float>(diameter, diameter, diameter * 2), center ) );
    // shadow->bakeShadowMap( scene );
    scene.mLights.push_back( shadow );

    // light green
    /*
    point.reset( new PointLight( center + fanVector3<float>(diameter, 0, diameter),
                            fanPixel( 255, 0, 255, 0 ) ) );
    shadow.reset( new ShadowUsingZBufferLight( point,
             fanVector3<float>(diameter, diameter, diameter * 1.5), center ) );
    shadow->bakeShadowMap( scene );
    scene.mLights.push_back( shadow);
    */

    // light blue
    /*
    point.reset( new PointLight( center + fanVector3<float>(0, -diameter, diameter),
                            fanPixel( 255, 0, 0, 255 ) ) );
    shadow.reset( new ShadowUsingZBufferLight( point,
             fanVector3<float>(diameter, diameter, diameter * 1.5), center ) );
    shadow->bakeShadowMap( scene );
    scene.mLights.push_back( shadow );
    */

    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    OrthogonalLens OrthoLens( center + fanVector3<float>(radius, radius, 0),
                              center,
                              center + fanVector3<float>(0, 0, 1),
                              fanVector3<float>(diameter*4.0f/3.0f, diameter, diameter * 1.5) );
    PerspectiveLens PerspLens( center + fanVector3<float>(radius, radius, 0),
                               center,
                               center + fanVector3<float>(0, 0, 1),
                               fanVector3<float>(diameter*2.0f/3.0f, diameter/2.0f, diameter * 1.5),
                               radius*1.3 );
    // enterLoop( OrthoLens, PerspLens, sdl, scene, radius );
    fan::fanCamera* currentCamera = NULL;
    fan::fanLens* currentLens = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;
    DepthCamera depthCamera;
    RasterisationCamera<FlatShadingFiller> flatShadingCamera;
    RasterisationCamera<GouraudShadingFiller> gouraudShadingCamera;
    RasterisationCamera<PhongShadingFiller> phongShadingCamera;

    bool done = false;
    bool refresh = false;

    currentCamera = &depthCamera;
    currentLens = &OrthoLens;
    Uint32 time;
    float radian = 0;
    while ( !done ) {
        time = SDL_GetTicks();
        radian += 0.01;
        point->mPos = center + fanVector3<float>(diameter*std::sin(radian), diameter*std::cos(radian), diameter);
        shadow->bakeShadowMap( scene );
        currentCamera->takePicture( scene, sdl, *currentLens );
        std::cout << SDL_GetTicks() - time << " ms in takePicture" << std::endl;
        sdl.develope();

        SDL_Event event;
        refresh = false;
        //while (SDL_WaitEvent(&event) && !refresh) {
        if ( SDL_PollEvent( &event ) ) {
            if ( event.type == SDL_QUIT ) {
                done = true;
                refresh = true;
            }
            switch( event.type ) {
                case SDL_QUIT:
                    done = true;
                break;
                case SDL_KEYDOWN:
                {
                    fan::fanVector<float, 3> move;
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
                        case SDLK_z: move[0] += radius*0.1; Translate( move, OrthoLens); Translate( move, PerspLens); break;
                        case SDLK_x: move[0] -= radius*0.1; Translate( move, OrthoLens); Translate( move, PerspLens); break;
                        case SDLK_c: move[1] += radius*0.1; Translate( move, OrthoLens); Translate( move, PerspLens); break;
                        case SDLK_v: move[1] -= radius*0.1; Translate( move, OrthoLens); Translate( move, PerspLens); break;
                        case SDLK_b: move[2] += radius*0.1; Translate( move, OrthoLens); Translate( move, PerspLens); break;
                        case SDLK_n: move[2] -= radius*0.1; Translate( move, OrthoLens); Translate( move, PerspLens); break;
                        case SDLK_ESCAPE: done = true; break;
                        case SDLK_1: currentCamera = &pointCamera; break;
                        case SDLK_2: currentCamera = &wireframeCamera; break;
                        case SDLK_3: currentCamera = &depthCamera; break;
                        case SDLK_4: currentCamera = &flatShadingCamera; break;
                        case SDLK_5: currentCamera = &gouraudShadingCamera; break;
                        case SDLK_6: currentCamera = &phongShadingCamera; break;
                        case SDLK_o: currentLens = &OrthoLens; break;
                        case SDLK_p: currentLens = &PerspLens; break;
                        default: continue; break;
                    }
                    refresh = true;
                }
            }
        }
    }
}

