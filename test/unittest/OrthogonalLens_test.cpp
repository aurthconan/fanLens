#include <gtest/gtest.h>

#include <lens/OrthogonalLens.h>
#include <fanTriangleMesh.h>
#include <objects/TriangleMeshObject.h>

#include <texture/film/SDLFilm.h>
#include <camera/PointScannerCamera.h>
#include <texture/film/FreeImageFilm.h>
#include <fanBufferObject.h>
#include <objects/TriangleMeshObject.h>
#include <fanScanner.h>

#include <boost/shared_ptr.hpp>

using namespace fan;

/*                 ^ z
 *                 |    ^ y
 *                 |   /
 *                 |  /
 *                 | /
 *                 |/
 *                 .------------> x
 *           ^ y  /
 *           |   /
 *    ---------------
 *   |       | / z   |
 *   |       |/      |
 *   |       .-------|--> x
 *   |               |
 *   |               |
 *    ---------------
 *   Screen coordinate
 */


TEST(OrthogonalLens,LookThroughPositiveYAxis) {
    OrthogonalLens lens( fanVector3<float>(0, -100, 0),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(100, 100, 100) );

    fanVector<int, 2> size;
    size[0] = 100; size[1] = 100;
    fanVector<float, 2> result;
    fanVector<float, 4> homoPos;
    fanMatrix<float, 4, 4> pos { 1,0,0,0,
                                 0,1,0,0,
                                 0,0,1,0,
                                 0,0,0,1 };
    project( transform( pos, fanVector3<float>(0, 0, 1) ),
                    lens, size,
                    result, homoPos );
    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 51, result[1] );

    project( transform( pos, fanVector3<float>( 0, 0, 2 ) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 52, result[1] );

    project( transform( pos, fanVector3<float>( 1, 0, 2 ) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 51, result[0] );
    EXPECT_EQ( 52, result[1] );

    project( transform( pos, fanVector3<float>( 2, 0, 2 ) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 52, result[0] );
    EXPECT_EQ( 52, result[1] );

}

/*                 ^ z                                ^
 *                 |    ^ y                   y ^    / x
 *                 |   /                        | /|/
 *                 |  /                         |/ |
 *                 | /                          / /|
 *                 |/                          /|/ |
 *                 .------------>-------------|-.  /
 *                /             x             |   /
 *                                            |  /
 *                                            | /
 *                                            |/
 *
 */

TEST(OrthogonalLens,LookThroughNegativeXAxis) {
    OrthogonalLens lens( fanVector3<float>(100, 0, 0),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(100, 100, 10000) );

    fanVector<int, 2> size;
    fanVector<float, 4> homoPos;
    fanMatrix<float, 4, 4> pos { 1,0,0,0,
                                 0,1,0,0,
                                 0,0,1,0,
                                 0,0,0,1 };
    size[0] = 100; size[1] = 100;
    fanVector<float, 2> result;
    project( transform( pos, fanVector3<float>(0, 0, 1) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 51, result[1] );

    project( transform( pos, fanVector3<float>( 0, 0, 2 ) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 52, result[1] );

    project( transform( pos, fanVector3<float>( 0, 1, 2 ) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 51, result[0] );
    EXPECT_EQ( 52, result[1] );

    project( transform( pos, fanVector3<float>( 0, 2, 2 ) ),
                    lens, size,
                    result, homoPos );

    EXPECT_EQ( 52, result[0] );
    EXPECT_EQ( 52, result[1] );

}

TEST(OrthogonalLens,RenderAxis) {
    OrthogonalLens lens( fanVector3<float>( 1000, -1000, 1000),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1),
                         fanVector3<float>(800, 600, 10000) );

    fan::fanVector<int, 2> size;
    size[0] = 800; size[1] = 600;
    FreeImageFilm film( size, "RenderAxis.png" );
    fan::fanScene scene;

    boost::shared_ptr<fan::fanBufferObject<fan::fanVector3<float> > >
        vertices( new fan::fanBufferObject<fan::fanVector3<float> >( 3000 ) );
    boost::shared_ptr<fan::fanBufferObject<fan::fanTriangle> >
        faces( new fan::fanBufferObject<fan::fanTriangle>( 0 ) );
    boost::shared_ptr<fan::fanTriangleMesh>
        mesh( new fan::fanTriangleMesh() );
    mesh->mVertices = vertices;
    mesh->mFaces = faces;

    boost::shared_ptr<TriangleMeshObject>
        object(new TriangleMeshObject(fanMatrix<float, 4, 4>{1,0,0,0,
                                                             0,1,0,0,
                                                             0,0,1,0,
                                                             0,0,0,1,} ) );
    object->mMeshes.push_back( mesh );
    int verticesNum = 0;
    for ( size_t i = 0; i < 1000; ++i ) {
        vertices->mBuffer[verticesNum] = fan::fanVector3<float>( i, 0, 0 );
        ++verticesNum;
    }
    for ( size_t i = 0; i < 1000; i+=4 ) {
        vertices->mBuffer[verticesNum] = fan::fanVector3<float>( 0, i, 0 );
        ++verticesNum;
    }
    for ( size_t i = 0; i < 1000; i+=8 ) {
        vertices->mBuffer[verticesNum] = fan::fanVector3<float>( 0, 0, i );
        ++verticesNum;
    }

    scene.mTriangleMeshObjects.push_back( object );
    PointScannerCamera camera;
    camera.takePicture( scene, film, lens );
    film.develope();

}

