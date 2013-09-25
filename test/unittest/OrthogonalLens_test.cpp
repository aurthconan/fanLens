#include <gtest/gtest.h>

#include <lens/OrthogonalLens.h>

#include <texture/film/SDLFilm.h>
#include <camera/PointScannerCamera.h>
#include <texture/film/FreeImageFilm.h>

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
    fanCamera camera;
    camera.project( fanVector3<float>(0, 0, 1),
                    lens, size,
                    result );
    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 51, result[1] );

    camera.project( fanVector3<float>( 0, 0, 2 ),
                    lens, size,
                    result );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 1, 0, 2 ),
                    lens, size,
                    result );

    EXPECT_EQ( 51, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 2, 0, 2 ),
                    lens, size,
                    result );

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
    size[0] = 100; size[1] = 100;
    fanVector<float, 2> result;
    camera.project( fanVector3<float>(0, 0, 1),
                    lens, size,
                    result );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 51, result[1] );

    camera.project( fanVector3<float>( 0, 0, 2 ),
                    lens, size,
                    result );

    EXPECT_EQ( 50, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 0, 1, 2 ),
                    lens, size,
                    result );

    EXPECT_EQ( 51, result[0] );
    EXPECT_EQ( 52, result[1] );

    camera.project( fanVector3<float>( 0, 2, 2 ),
                    lens, size,
                    result );

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
    for ( size_t i = 0; i < 1000; ++i ) {
        scene.mVertices.push_back( fan::fanVector3<float>( i, 0, 0 ) );
    }
    for ( size_t i = 0; i < 1000; i+=4 ) {
        scene.mVertices.push_back( fan::fanVector3<float>( 0, i, 0 ) );
    }
    for ( size_t i = 0; i < 1000; i+=8 ) {
        scene.mVertices.push_back( fan::fanVector3<float>( 0, 0, i ) );
    }

    PointScannerCamera camera;
    camera.takePicture( scene, film, lens );
    film.develope();

}

