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
                         fanVector3<float>(0, 0, 1) );

    fanVector<int, 2> size;
    size[0] = 100; size[1] = 100;
    fanTexture<int, 2> target( size );
    fanVector<int, 2> dot;
    fanVector<float, 2> result;
    lens.project( result,
                  fanVector3<float>(0, 0, 1),
                  target );
    dot = result;
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 51, dot[1] );

    lens.project( result,
                  fanVector3<float>( 0, 0, 2 ),
                  target );
    dot = result;
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( result,
                  fanVector3<float>( 1, 0, 2 ),
                  target );
    dot = result;
    EXPECT_EQ( 51, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( result,
                  fanVector3<float>( 2, 0, 2 ),
                  target );
    dot = result;
    EXPECT_EQ( 52, dot[0] );
    EXPECT_EQ( 52, dot[1] );

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
                         fanVector3<float>(0, 0, 1) );

    fanVector<int, 2> size;
    size[0] = 100; size[1] = 100;
    fanTexture<int, 2> target( size );
    fanVector<int, 2> dot;
    fanVector<float, 2> result;
    lens.project( result,
                  fanVector3<float>(0, 0, 1),
                  target );
    dot = result;
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 51, dot[1] );

    lens.project( result,
                  fanVector3<float>( 0, 0, 2 ),
                  target );
    dot = result;
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( result,
                  fanVector3<float>( 0, 1, 2 ),
                  target );
    dot = result;
    EXPECT_EQ( 51, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( result,
                  fanVector3<float>( 0, 2, 2 ),
                  target );
    dot = result;
    EXPECT_EQ( 52, dot[0] );
    EXPECT_EQ( 52, dot[1] );

}

TEST(OrthogonalLens,RenderAxis) {
    OrthogonalLens lens( fanVector3<float>( 1, -1, 1),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 0, 1) );

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

