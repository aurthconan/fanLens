#include "LoadFile.h"

#include <string>
#include <iostream>

using namespace std;
using namespace fan;

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/shared_ptr.hpp>

#include <fanTriangleMesh.h>
#include <objects/TriangleMeshObject.h>
#include <fanScene.h>

namespace {

using namespace Assimp;
bool loadPly( char* file,
              fanScene& outScene,
              fanVector3<float>& boundBoxMin,
              fanVector3<float>& boundBoxMax )
{
    Importer importer;
    const aiScene* scene = importer.ReadFile( file, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate );
    if ( !scene ) {
        return false;
    }
    cout << " scene->mNumMeshes " <<  scene->mNumMeshes << endl;
    vector<boost::shared_ptr<fanTriangleMesh> > meshes;
    boost::shared_ptr<fanTriangleMesh> fanMesh;
    bool fixNormals = false;
    for ( size_t i = 0, max = scene->mNumMeshes;
                i < max; ++i ) {
        aiMesh* mesh = scene->mMeshes[i];
        cout << " mesh->mNumVertices " <<  mesh->mNumVertices << endl;
        fanMesh.reset( new fanTriangleMesh() );
        meshes.push_back( fanMesh );
        boost::shared_ptr<fanBufferObject<fanVector3<float> > >
                vertice( new fanBufferObject<fanVector3<float> >(mesh->mNumVertices) );
        boost::shared_ptr<fanBufferObject<fanVector3<float> > >
                normal( new fanBufferObject<fanVector3<float> >(mesh->mNumVertices) );
        fanMesh->mVertices = vertice;
        fanMesh->mNormals = normal;
        if ( mesh->mNormals == NULL ) {
            fixNormals = true;
        }
        for ( size_t j = 0, max = mesh->mNumVertices;
                j < max; ++j ) {
            aiVector3D vector = mesh->mVertices[j];
            vertice->mBuffer[j] = fanVector3<float>(vector.x,
                                                    vector.y,
                                                    vector.z);
            if ( vector.x > boundBoxMax[0] ) {
                boundBoxMax[0] = vector.x;
            }
            if ( vector.x < boundBoxMin[0] ) {
                boundBoxMin[0] = vector.x;
            }
            if ( vector.y > boundBoxMax[1] ) {
                boundBoxMax[1] = vector.y;
            }
            if ( vector.y < boundBoxMin[1] ) {
                boundBoxMin[1] = vector.y;
            }
            if ( vector.z > boundBoxMax[2] ) {
                boundBoxMax[2] = vector.z;
            }
            if ( vector.z < boundBoxMin[2] ) {
                boundBoxMin[2] = vector.z;
            }
            if ( !fixNormals ) {
                vector = mesh->mNormals[j];
                normal->mBuffer[j] = normalize(fanVector3<float>(vector.x,
                                                                 vector.y,
                                                                 vector.z));
            }
        }
        cout << " mesh->mNumFaces " <<  mesh->mNumFaces << endl;
        boost::shared_ptr<fanBufferObject<fanTriangle> >
                face( new fanBufferObject<fanTriangle>(mesh->mNumFaces) );
        fanMesh->mFaces = face;
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
    }
    // fix normals
    for ( size_t i = 0, max = fanMesh->mFaces->mSize; i < max; ++i ) {
        fanTriangle& triangle = fanMesh->mFaces->mBuffer[i];
        fanMesh->mNormals->mBuffer[ triangle.pointsIndex[0] ] += triangle.mNormal;
        fanMesh->mNormals->mBuffer[ triangle.pointsIndex[1] ] += triangle.mNormal;
        fanMesh->mNormals->mBuffer[ triangle.pointsIndex[2] ] += triangle.mNormal;
    }
    for ( size_t i = 0, max = fanMesh->mVertices->mSize; i < max; ++i ) {
        fanMesh->mNormals->mBuffer[i] = normalize(fanMesh->mNormals->mBuffer[i]);
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
        boost::shared_ptr<TriangleMeshObject>
            object( new TriangleMeshObject(
                        fanMatrix< float, 4, 4>{pos.a1,pos.a2,pos.a3,pos.a4,
                                                pos.b1,pos.b2,pos.b3,pos.b4,
                                                pos.c1,pos.c2,pos.c3,pos.c4,
                                                pos.d1,pos.d2,pos.d3,pos.d4, } ) );
        cout << " node->mNumMeshes " << node->mNumMeshes << endl;
        for ( size_t i = 0; i < node->mNumMeshes; ++i ) {
            object->mMeshes.push_back( meshes[i] );
        }
        outScene.mTriangleMeshObjects.push_back( object );
    }

    return true;
}

}

bool loadFile( char* file,
               fan::fanScene& outScene,
               fan::fanVector3<float>& boundBoxMin,
               fan::fanVector3<float>& boundBoxMax )
{
    string filename( file );

    size_t extensionSeperatorPos = filename.rfind( '.' );
    if ( extensionSeperatorPos == string::npos ) {
        cerr << "Could not find file extension." << std::endl;
        return false;
    }
    string filenameExtension = filename.substr( extensionSeperatorPos + 1,
                                                filename.length() );

    if ( filenameExtension == "ply" ) {
        return loadPly( file, outScene, boundBoxMin, boundBoxMax );
    }
}

