#include <SDL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

using namespace std;

using namespace fan;

bool loadFile( char* file, fanScene& scene );

int main(int argc, char** argv) {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }
    atexit(SDL_Quit);

    fanScene scene;

    if ( argc < 2 ) {
        return 2;
    }

    if ( !loadFile( argv[1], scene ) ) {
        return 3;
    }

    fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    SDLFilm sdl( size );

    OrthogonalLens OrthoLens( fanVector3<float>( 400, -400, 400),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(800, 600, 10000000) );
    PerspectiveLens PerspLens( fanVector3<float>(400, -400, 400),
                          fanVector3<float>(0, 0, 0),
                          fanVector3<float>(0, 0, 1),
                          fanVector3<float>(800, 600, 10000000),
                          565 );


    fanCamera* currentCamera = NULL;
    fanLens* currentLens = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;

    bool done = false;
    bool refresh = false;

    currentCamera = &pointCamera;
    currentLens = &OrthoLens;
    StopWatch stopWatch;
    while ( !done ) {
        stopWatch.start();
        currentCamera->takePicture( scene, sdl, *currentLens );
        stopWatch.stop("Take Picture");
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
                    fanVector<float, 3> move;
                    switch( event.key.keysym.sym ) {
                        case SDLK_w: move[1] += 1; break;
                        case SDLK_s: move[1] -= 1; break;
                        case SDLK_a: move[0] += 1; break;
                        case SDLK_d: move[0] -= 1; break;
                        case SDLK_ESCAPE: done = true; break;
                        case SDLK_1: currentCamera = &pointCamera; break;
                        case SDLK_2: currentCamera = &wireframeCamera; break;
                        case SDLK_o: currentLens = &OrthoLens; break;
                        case SDLK_p: currentLens = &PerspLens; break;
                        default: continue; break;
                    }
                    OrthoLens.move( move );
                    PerspLens.move( move );
                    refresh = true;
                }
            }
        }
    }
}

using namespace Assimp;
bool loadFile( char* file, fanScene& ourScene ) {
    Importer importer;
    const aiScene* scene = importer.ReadFile( file, aiProcess_Triangulate );
    if ( !scene ) {
        return false;
    }
    cout << " scene->mNumMeshes " <<  scene->mNumMeshes << endl;
    for ( size_t i = 0, max = scene->mNumMeshes;
                i < max; ++i ) {
        aiMesh* mesh = scene->mMeshes[i];
        cout << " mesh->mNumVertices " <<  mesh->mNumVertices << endl;
        for ( size_t j = 0, max = mesh->mNumVertices;
                j < max; ++j ) {
            aiVector3D vector = mesh->mVertices[j];
            ourScene.mVertices.push_back(fanVector3<float>(vector.x * 400,
                                                           vector.y * 400,
                                                           vector.z * 400));
        }
    }
    size_t verticesNum = 0;
    for ( size_t i = 0, max = scene->mNumMeshes;
                i < max; ++i ) {
        aiMesh* mesh = scene->mMeshes[i];
        cout << " mesh->mNumVertices " <<  mesh->mNumVertices << endl;
        cout << " mesh->mNumFaces " <<  mesh->mNumFaces << endl;
        for ( size_t j = 0, max = mesh->mNumFaces;
                j < max; ++j ) {
            aiFace face = mesh->mFaces[j];
            if ( face.mNumIndices != 3 ) {
                continue;
            }
            unsigned int aIndex = face.mIndices[0] + verticesNum;
            unsigned int bIndex = face.mIndices[1] + verticesNum;
            unsigned int cIndex = face.mIndices[2] + verticesNum;
            fanTriangle t( &ourScene.mVertices[aIndex],
                           &ourScene.mVertices[bIndex],
                           &ourScene.mVertices[cIndex] );

            ourScene.mTriangles.push_back( t );
        }
        verticesNum += mesh->mNumVertices;
    }

    /*
    for ( size_t i = 0; i < 1000; ++i ) {
        ourScene.mVertices.push_back( fan::fanVector3<float>( i, 0, 0 ) );
    }
    for ( size_t i = 0; i < 1000; i+=4 ) {
        ourScene.mVertices.push_back( fan::fanVector3<float>( 0, i, 0 ) );
    }
    for ( size_t i = 0; i < 1000; i+=8 ) {
        ourScene.mVertices.push_back( fan::fanVector3<float>( 0, 0, i ) );
    }
    */

    return true;
}

