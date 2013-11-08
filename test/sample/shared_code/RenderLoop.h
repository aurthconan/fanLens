#ifndef RENDERLOOP_H

#define RENDERLOOP_H

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
#include <objects/TriangleMeshObject.h>

inline void enterLoop( fan::fanLens& OrthoLens,
                       fan::fanLens& PerspLens,
                       fan::fanFilm& sdl,
                       fan::fanScene& scene,
                       float radius )
{
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
    while ( !done ) {
        time = SDL_GetTicks();
        currentCamera->takePicture( scene, sdl, *currentLens );
        std::cout << SDL_GetTicks() - time << " ms in takePicture" << std::endl;
        sdl.develope();

        SDL_Event event;
        refresh = false;
        while (!refresh && SDL_WaitEvent(&event)) {
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

#endif /* end of include guard: RENDERLOOP_H */
