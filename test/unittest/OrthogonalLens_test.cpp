#include <gtest/gtest.h>

#include <lens/OrthogonalLens.h>
#include <fanTriangleMesh.h>

#include <texture/film/SDLFilm.h>
#include <camera/PointScannerCamera.h>
#include <texture/film/FreeImageFilm.h>
#include <fanBufferObject.h>

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
    fanCamera camera;
    camera.project( fanVector3<float>(0, 0, 1),
                    lens.mPos, lens.mViewTransformation, size,
                    result, homoPos );
    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 51, result[1] );

    camera.project( fanVector3<float>( 0, 0, 2 ),
                    lens.mPos, lens.mViewTransformation, size,
                    result, homoPos );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 1, 0, 2 ),
                    lens.mPos, lens.mViewTransformation, size,
                    result, homoPos );

    EXPECT_EQ( 51, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 2, 0, 2 ),
                    lens.mPos, lens.mViewTransformation, size,
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

    fanCamera camera;
    fanVector<int, 2> size;
    fanVector<float, 4> homoPos;
    size[0] = 100; size[1] = 100;
    fanVector<float, 2> result;
    camera.project( fanVector3<float>(0, 0, 1),
                    lens.mPos, lens.mViewTransformation, size,
                    result, homoPos );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 51, result[1] );

    camera.project( fanVector3<float>( 0, 0, 2 ),
                    lens.mPos, lens.mViewTransformation, size,
                    result, homoPos );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 0, 1, 2 ),
                    lens.mPos, lens.mViewTransformation, size,
                    result, homoPos );

    EXPECT_EQ( 51, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 0, 2, 2 ),
                    lens.mPos, lens.mViewTransformation, size,
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

    fanTriangleMesh mesh(fanMatrix<float, 4, 4>{1,0,0,0,
                                                0,1,0,0,
                                                0,0,1,0,
                                                0,0,0,1,} );
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
    mesh.mVertices.push_back( vertices );
    mesh.mFaces.push_back( faces );

    scene.mTriangleMeshes.push_back( mesh );
    PointScannerCamera camera;
    camera.takePicture( scene, film, lens );
    film.develope();

}

