#include <gtest/gtest.h>

#include <lens/OrthogonalLens.h>

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
    lens.project( dot,
                  fanVector3<float>(0, 0, 1),
                  target );
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 51, dot[1] );

    lens.project( dot,
                  fanVector3<float>( 0, 0, 2 ),
                  target );
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( dot,
                  fanVector3<float>( 1, 0, 2 ),
                  target );
    EXPECT_EQ( 51, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( dot,
                  fanVector3<float>( 2, 0, 2 ),
                  target );
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
    lens.project( dot,
                  fanVector3<float>(0, 0, 1),
                  target );
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 51, dot[1] );

    lens.project( dot,
                  fanVector3<float>( 0, 0, 2 ),
                  target );
    EXPECT_EQ( 50, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( dot,
                  fanVector3<float>( 0, 1, 2 ),
                  target );
    EXPECT_EQ( 51, dot[0] );
    EXPECT_EQ( 52, dot[1] );

    lens.project( dot,
                  fanVector3<float>( 0, 2, 2 ),
                  target );
    EXPECT_EQ( 52, dot[0] );
    EXPECT_EQ( 52, dot[1] );

}

