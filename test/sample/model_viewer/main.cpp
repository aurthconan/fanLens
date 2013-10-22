#include <SDL.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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

#include <cstdlib>

#include <vector>
#include <iostream>
#include <limits>
#include <cmath>

#include <boost/shared_ptr.hpp>

using namespace std;

using namespace fan;

float xMin, xMax;
float yMin, yMax;
float zMin, zMax;
float radius;

bool loadFile( char* file, fanScene& scene,
                vector<boost::shared_ptr<fanBufferObject<fanVector3<float> > > >& vertices,
                vector<boost::shared_ptr<fanBufferObject<fanTriangle> > >& faces );

int main(int argc, char** argv) {
    if ( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        return 1;
    }
    atexit(SDL_Quit);

    fanScene scene;

    if ( argc < 2 ) {
        return 2;
    }

    xMin = yMin = zMin = std::numeric_limits<float>::max();
    xMax = yMax = zMax = std::numeric_limits<float>::min();
    vector<boost::shared_ptr<fanBufferObject<fanVector3<float> > > > vertices;
    vector<boost::shared_ptr<fanBufferObject<fanTriangle> > > faces;

    if ( !loadFile( argv[1], scene, vertices, faces ) ) {
        faces.clear();
        vertices.clear();

        return 3;
    }

    float xBound = xMax - xMin;
    float yBound = yMax - yMin;
    float zBound = zMax - zMin;
    fanVector3<float> center(xMin + xBound/2, yMin + yBound/2, zMin + zBound/2);
    float diameter = sqrt( xBound*xBound + yBound*yBound + zBound*zBound );
    radius = diameter / 2.0f;

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


    fanCamera* currentCamera = NULL;
    fanLens* currentLens = NULL;
    PointScannerCamera pointCamera;
    WireframeCamera wireframeCamera;
    DepthCamera depthCamera;

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
                        case SDLK_o: currentLens = &OrthoLens; break;
                        case SDLK_p: currentLens = &PerspLens; break;
                        default: continue; break;
                    }
                    refresh = true;
                }
            }
        }
    }

    faces.clear();
    vertices.clear();
}

using namespace Assimp;
bool loadFile( char* file, fanScene& ourScene,
               vector<boost::shared_ptr<fanBufferObject<fanVector3<float> > > >& vertices,
               vector<boost::shared_ptr<fanBufferObject<fanTriangle> > >& faces ) {
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
        boost::shared_ptr<fanBufferObject<fanVector3<float> > >
                vertice( new fanBufferObject<fanVector3<float> >(mesh->mNumVertices) );
        for ( size_t j = 0, max = mesh->mNumVertices;
                j < max; ++j ) {
            aiVector3D vector = mesh->mVertices[j];
            vertice->mBuffer[j] = fanVector3<float>(vector.x,
                                                    vector.y,
                                                    vector.z);
            if ( vector.x > xMax ) {
                xMax = vector.x;
            }
            if ( vector.x < xMin ) {
                xMin = vector.x;
            }
            if ( vector.y > yMax ) {
                yMax = vector.y;
            }
            if ( vector.y < yMin ) {
                yMin = vector.y;
            }
            if ( vector.z > zMax ) {
                zMax = vector.z;
            }
            if ( vector.z < zMin ) {
                zMin = vector.z;
            }
        }
        vertices.push_back( vertice );
        cout << " mesh->mNumFaces " <<  mesh->mNumFaces << endl;
        boost::shared_ptr<fanBufferObject<fanTriangle> >
                face( new fanBufferObject<fanTriangle>(mesh->mNumFaces) );
        for ( size_t j = 0, max = mesh->mNumFaces;
                j < max; ++j ) {
            aiFace aiface = mesh->mFaces[j];
            if ( aiface.mNumIndices != 3 ) {
                continue;
            }
            unsigned int aIndex = aiface.mIndices[0];
            unsigned int bIndex = aiface.mIndices[1];
            unsigned int cIndex = aiface.mIndices[2];
            face->mBuffer[j] = fanTriangle( &vertice->mBuffer[aIndex],
                                            &vertice->mBuffer[bIndex],
                                            &vertice->mBuffer[cIndex],
                                            aIndex, bIndex, cIndex );
        }
        faces.push_back( face );
    }
    vector<aiNode*> nodes;
    nodes.push_back(scene->mRootNode);
    while ( nodes.size() != 0 ) {
        aiNode* node = nodes[nodes.size()-1];
        nodes.pop_back();
        if ( node == NULL ) {
            continue;
        }
        cout << " node->mNumChildren " << node->mNumChildren << endl;
        for ( size_t i = 0; i < node->mNumChildren; ++i ) {
            nodes.push_back( node->mChildren[i] );
        }
        aiMatrix4x4 pos = node->mTransformation;
        fanTriangleMesh object( fanMatrix< float, 4, 4>{pos.a1,pos.a2,pos.a3,pos.a4,
                                                        pos.b1,pos.b2,pos.b3,pos.b4,
                                                        pos.c1,pos.c2,pos.c3,pos.c4,
                                                        pos.d1,pos.d2,pos.d3,pos.d4,
                                                        } );
        cout << " node->mNumMeshes " << node->mNumMeshes << endl;
        for ( size_t i = 0; i < node->mNumMeshes; ++i ) {
            object.mVertices.push_back( vertices[i] );
            object.mFaces.push_back( faces[i] );
        }
        ourScene.mTriangleMeshes.push_back( object );
    }

    return true;
}

