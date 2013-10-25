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

using namespace fan;
using namespace std;

void addTriangle( fanTriangleMesh& cube, size_t startIndex, int& facesNum ) {
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
            fanTriangle t1( &cube.mVertices->mBuffer[p0Index],
                            &cube.mVertices->mBuffer[p2Index],
                            &cube.mVertices->mBuffer[p1Index],
                            p0Index, p2Index, p1Index );
            fanTriangle t2( &cube.mVertices->mBuffer[p1Index],
                            &cube.mVertices->mBuffer[p2Index],
                            &cube.mVertices->mBuffer[p3Index],
                            p1Index, p2Index, p3Index );
            cube.mFaces->mBuffer[facesNum] = t1;
            ++facesNum;
            cube.mFaces->mBuffer[facesNum] = t2;
            ++facesNum;
        }
    }
}

int main() {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }
    fanMatrix<float, 4, 4> cubePos { 1, 0, 0, 0,
                                     0, 1, 0, 0,
                                     0, 0, 1, 0,
                                     0, 0, 0, 1 };
    boost::shared_ptr<fanBufferObject<fanVector3<float> > >
        vertices( new fanBufferObject<fanVector3<float> >( 6*5*5 ) );
    boost::shared_ptr<fanBufferObject<fanTriangle> >
        faces( new fanBufferObject<fanTriangle>( 4*4*2*6 ) );

    boost::shared_ptr<fanTriangleMesh> cube( new fanTriangleMesh( ) );
    cube->mVertices = vertices;
    cube->mFaces = faces;

    boost::shared_ptr<TriangleMeshObject>
        cubeObject( new TriangleMeshObject( cubePos ) );
    cubeObject->mMeshes.push_back( cube );

    fanScene scene;

    int verticesNum = 0;
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            cube->mVertices->mBuffer[verticesNum] = fanVector3<float>( i, j, 0 );
            verticesNum++;
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            cube->mVertices->mBuffer[verticesNum] = fanVector3<float>( i, 0, j );
            verticesNum++;
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            cube->mVertices->mBuffer[verticesNum] = fanVector3<float>( 0, i, j );
            verticesNum++;
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            cube->mVertices->mBuffer[verticesNum] = fanVector3<float>( i, j, 200 );
            verticesNum++;
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            cube->mVertices->mBuffer[verticesNum] = fanVector3<float>( i, 200, j );
            verticesNum++;
        }
    }
    for ( size_t i = 0; i <= 200; i += 50 ) {
        for ( size_t j = 0; j <= 200; j += 50 ) {
            cube->mVertices->mBuffer[verticesNum] = fanVector3<float>( 200, i, j );
            verticesNum++;
        }
    }
    int facesNum = 0;
    addTriangle( *cube, 0, facesNum );
    addTriangle( *cube, 25, facesNum );
    addTriangle( *cube, 50, facesNum );
    addTriangle( *cube, 75, facesNum );
    addTriangle( *cube, 100, facesNum );
    addTriangle( *cube, 125, facesNum );

    scene.mTriangleMeshObjects.push_back( cubeObject );

    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    OrthogonalLens OrthoLens( fanVector3<float>(400, 0, 0),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(800, 600, 800) );
    PerspectiveLens PerspLens( fanVector3<float>(400, 0, 0),
                          fanVector3<float>(0, 0, 0),
                          fanVector3<float>(0, 0, 1),
                          fanVector3<float>(800, 600, 800),
                          3000 );

    fanCamera* currentCamera = NULL;
    fanLens* currentLens = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;
    DepthCamera depthCamera;

    bool done = false;

    currentCamera = &pointCamera;
    currentLens = &PerspLens;
    Uint32 time;
    while ( !done ) {
        time = SDL_GetTicks();
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
                        case SDLK_p: currentLens = &PerspLens; break;
                    }
                }
                break;
            }
        }
    }

    atexit(SDL_Quit);
}

