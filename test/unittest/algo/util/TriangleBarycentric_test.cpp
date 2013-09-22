#include <gtest/gtest.h>

#include <algo/util/TriangleBarycentric.h>

using namespace fan;

TEST(TriangleBarycentric,SimpleTest) {
    fanVector<float, 2> p1, p2, p3, p;
    p1[0] = 0; p1[1] = 0;
    p2[0] = 100; p2[1] = 0;
    p3[0] = 0; p3[1] = 100;

    fanVector<float, 3> result;

    result = ComputeTriangleBarycentricCoord( p1, p2, p3, p1 );
    EXPECT_FLOAT_EQ( 1.0f, result[0] );
    EXPECT_FLOAT_EQ( .0f, result[1] );
    EXPECT_FLOAT_EQ( .0f, result[2] );

    result = ComputeTriangleBarycentricCoord( p1, p2, p3, p2 );
    EXPECT_FLOAT_EQ( .0f, result[0] );
    EXPECT_FLOAT_EQ( 1.0f, result[1] );
    EXPECT_FLOAT_EQ( .0f, result[2] );

    result = ComputeTriangleBarycentricCoord( p1, p2, p3, p3 );
    EXPECT_FLOAT_EQ( .0f, result[0] );
    EXPECT_FLOAT_EQ( .0f, result[1] );
    EXPECT_FLOAT_EQ( 1.0f, result[2] );

    p[0] = 50;
    result = ComputeTriangleBarycentricCoord( p1, p2, p3, p );
    EXPECT_FLOAT_EQ( .5f, result[0] );
    EXPECT_FLOAT_EQ( .5f, result[1] );
    EXPECT_FLOAT_EQ( .0f, result[2] );

}

