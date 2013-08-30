#include <gtest/gtest.h>

#include <lens/OrthogonalLens.h>

using namespace fan;

TEST(OrthogonalLens,LookThroughPositiveZAxis) {
    OrthogonalLens lens( fanVector3<float>(0, 0, -100),
                         fanVector3<float>(0, 0, 0),
                         fanVector3<float>(0, 1, 0) );

    fanVector<int, 2> size;
    size[0] = 100; size[1] = 100;
    fanTexture<int, 2> target( size );
    fanVector<int, 2> dot = lens.project( fanVector3<float>(0, 0, 1),
                                          target );
    EXPECT_EQ( 0, dot[0] );
    EXPECT_EQ( 0, dot[1] );

    dot = lens.project( fanVector3<float>( 1, 1, 2 ),
                        target );
    EXPECT_EQ( 1, dot[0] );
    EXPECT_EQ( 1, dot[1] );
}

