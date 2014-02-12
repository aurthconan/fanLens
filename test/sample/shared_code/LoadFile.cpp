#include "LoadFile.h"

#include <algorithm>
#include <string>
#include <iostream>

using namespace std;
using namespace fan;

#if ENABLE_ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif  // ENABLE_ASSIMP

#if ENABLE_LIB3DS
#include <lib3ds.h>
#endif  // ENABLE_LIB3DS

#include <boost/shared_ptr.hpp>

#include <fanTriangleMesh.h>
#include <objects/TriangleMeshObject.h>
#include <fanScene.h>

#include <debug/Debug.h>

namespace {

#if ENABLE_ASSIMP
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
        cout << " objectToWorld " << object->mObjectToWorld << endl;
        for ( size_t i = 0; i < node->mNumMeshes; ++i ) {
            object->mMeshes.push_back( meshes[i] );
        }
        outScene.mTriangleMeshObjects.push_back( object );
    }

    return true;
}
#endif  // ENABLE_ASSIMP

#if ENABLE_LIB3DS
bool load3ds( char* file,
              fanScene& outScene,
              fanVector3<float>& boundBoxMin,
              fanVector3<float>& boundBoxMax )
{
    bool loadResult = false;
    Lib3dsFile* _3dsFile = NULL;
    do {
        _3dsFile = lib3ds_file_open( file );
        if ( !_3dsFile ) {
            cerr << "load fail " << file << std::endl;
            break;
        }
        vector<boost::shared_ptr<fanTriangleMesh> > meshes;
        boost::shared_ptr<fanTriangleMesh> fanMesh;
        cout << "nmeshes " << _3dsFile->nmeshes << std::endl;
        for ( int i = 0, max = _3dsFile->nmeshes;
                    i < max; ++i ) {
            Lib3dsMesh* mesh = _3dsFile->meshes[i];
            cout << "nvertices " << mesh->nvertices << std::endl;

            fanMesh.reset( new fanTriangleMesh() );
            meshes.push_back( fanMesh );

            fanMesh->mVertices.reset(
                    new fanBufferObject<fanVector3<float> >( mesh->nvertices ) );
            fanMesh->mFaces.reset(
                    new fanBufferObject<fanTriangle>( mesh->nfaces ) );
            fanMesh->mNormals.reset(
                    new fanBufferObject<fanVector3<float> >( mesh->nvertices ) );

            for ( int j = 0, max = mesh->nvertices;
                    j < max; ++j ) {
                (fanMesh->mVertices)->mBuffer[j] =
                    fanVector3<float>( mesh->vertices[j][0],
                                       mesh->vertices[j][1],
                                       mesh->vertices[j][2] );
                if ( mesh->vertices[j][0] > boundBoxMax[0] ) {
                    boundBoxMax[0] = mesh->vertices[j][0];
                }
                if ( mesh->vertices[j][0] < boundBoxMin[0] ) {
                    boundBoxMin[0] = mesh->vertices[j][0];
                }
                if ( mesh->vertices[j][1] > boundBoxMax[1] ) {
                    boundBoxMax[1] = mesh->vertices[j][1];
                }
                if ( mesh->vertices[j][1] < boundBoxMin[1] ) {
                    boundBoxMin[1] = mesh->vertices[j][1];
                }
                if ( mesh->vertices[j][2] > boundBoxMax[2] ) {
                    boundBoxMax[2] = mesh->vertices[j][2];
                }
                if ( mesh->vertices[j][2] < boundBoxMin[2] ) {
                    boundBoxMin[2] = mesh->vertices[j][2];
                }

            }
            float (*vertices_normals)[3] = (float(*)[3])malloc(sizeof(float) * 9 * mesh->nfaces);
            float (*face_normals)[3] = (float(*)[3])malloc(sizeof(float) * 3 * mesh->nfaces);
            lib3ds_mesh_calculate_vertex_normals( mesh, vertices_normals );
            lib3ds_mesh_calculate_face_normals( mesh, face_normals );
            cout << "nfaces " << mesh->nfaces << std::endl;
            for ( int j = 0, max = mesh->nfaces;
                    j < max; ++j ) {
                Lib3dsFace face = mesh->faces[j];
                (fanMesh->mFaces)->mBuffer[j] =
                    fanTriangle( &(fanMesh->mVertices->mBuffer[face.index[0]]),
                                 &(fanMesh->mVertices->mBuffer[face.index[1]]),
                                 &(fanMesh->mVertices->mBuffer[face.index[2]]),
                                 face.index[0], face.index[1], face.index[2],
                                 fanVector3<float>(),
                                 fanVector3<float>( face_normals[j][0],
                                                    face_normals[j][1],
                                                    face_normals[j][2] ) );
                fanMesh->mNormals->mBuffer[ face.index[0] ] =
                    fanVector3<float>( vertices_normals[3*j][0],
                                       vertices_normals[3*j][1],
                                       vertices_normals[3*j][2] );
                fanMesh->mNormals->mBuffer[ face.index[1] ] =
                    fanVector3<float>( vertices_normals[3*j+1][0],
                                       vertices_normals[3*j+1][1],
                                       vertices_normals[3*j+1][2] );
                fanMesh->mNormals->mBuffer[ face.index[2] ] =
                    fanVector3<float>( vertices_normals[3*j+2][0],
                                       vertices_normals[3*j+2][1],
                                       vertices_normals[3*j+2][2] );
            }
            free( vertices_normals );
            free( face_normals );
        }

        vector<Lib3dsNode*> nodes;
        nodes.push_back(_3dsFile->nodes);
        while ( nodes.size() != 0 ) {
            Lib3dsNode* node = nodes[nodes.size()-1];
            nodes.pop_back();

            if ( node == NULL ) {
                continue;
            }

            nodes.push_back( node->childs );
            nodes.push_back( node->next );

            if ( node->type == LIB3DS_NODE_MESH_INSTANCE ) {
            /*
            Lib3dsMeshInstanceNode* meshNode = (Lib3dsMeshInstanceNode*) node;
            cerr << node->name << " pos " <<
                meshNode->pos[0] << "," << meshNode->pos[1] << "," << meshNode->pos[2] << std::endl;
            cerr << "    rot " << meshNode->rot[0] << "," << meshNode->rot[1] << "," << meshNode->rot[2] << "," << meshNode->rot[3] << std::endl;
            */
                int meshIndex = lib3ds_file_mesh_by_name( _3dsFile, node->name );
                if ( meshIndex != -1 ) {
                    boost::shared_ptr<TriangleMeshObject>
                        object( new TriangleMeshObject(
                                    fanMatrix< float, 4, 4>{ 1,0,0,0,
                                                             0,1,0,0,
                                                             0,0,1,0,
                                                             0,0,0,1, } ) );
                    object->mMeshes.push_back( meshes[meshIndex] );
                    outScene.mTriangleMeshObjects.push_back( object );
                }
            }
        }
        cout << "triangleMeshObjects.size() " << outScene.mTriangleMeshObjects.size() << std::endl;
        loadResult = true;
    } while ( false );

    if ( _3dsFile ) {
        lib3ds_file_free( _3dsFile );
    }
    return loadResult;
}
#endif  // ENABLE_LIB3DS

}

bool loadFile( char* file,
               fan::fanScene& outScene,
               fan::fanVector3<float>& boundBoxMin,
               fan::fanVector3<float>& boundBoxMax )
{
    (void) outScene; (void) boundBoxMax; (void) boundBoxMin;
    string filename( file );

    size_t extensionSeperatorPos = filename.rfind( '.' );
    if ( extensionSeperatorPos == string::npos ) {
        cerr << "Could not find file extension." << std::endl;
        return false;
    }
    string filenameExtension = filename.substr( extensionSeperatorPos + 1,
                                                filename.length() );
    std::transform( filenameExtension.begin(), filenameExtension.end(),
                    filenameExtension.begin(), ::tolower );

#if ENABLE_ASSIMP
    if ( filenameExtension == "ply" ) {
        return loadPly( file, outScene, boundBoxMin, boundBoxMax );
    } else
#endif  // ENABLE_ASSIMP
#if ENABLE_LIB3DS
    if ( filenameExtension == "3ds" ) {
        return load3ds( file, outScene, boundBoxMin, boundBoxMax );
    } else
#endif  // ENABLE_LIB3DS
    {
        cerr << "file type " << filenameExtension << " not supported." << std::endl;
    }
    return false;
}

