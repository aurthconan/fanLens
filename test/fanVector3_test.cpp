#include <gtest/gtest.h>

#include <fanVector3.h>

using namespace fan;

TEST(fanVector3,Constructor_int) {
    fanVector3<int> _variable;
    EXPECT_EQ( 0, _variable.x );
    EXPECT_EQ( 0, _variable.y );
    EXPECT_EQ( 0, _variable.z );

    fanVector3<int> _variable2( 1, 2, 3 );
    EXPECT_EQ( 1, _variable2.x );
    EXPECT_EQ( 2, _variable2.y );
    EXPECT_EQ( 3, _variable2.z );
}

TEST(fanVector3,Constructor_float) {
    fanVector3<float> _variable;
    EXPECT_FLOAT_EQ( .0f, _variable.x );
    EXPECT_FLOAT_EQ( .0f, _variable.y );
    EXPECT_FLOAT_EQ( .0f, _variable.z );

    fanVector3<float> _variable2( .1f, .2f, .3f );
    EXPECT_FLOAT_EQ( .1f, _variable2.x );
    EXPECT_FLOAT_EQ( .2f, _variable2.y );
    EXPECT_FLOAT_EQ( .3f, _variable2.z );
}

TEST(fanVector3,addOperator) {
    fanVector3<int> result = fanVector3<int>( 1, 2, 3 ) + fanVector3<int>( 4, 5, 6 );
    EXPECT_EQ( 5, result.x );
    EXPECT_EQ( 7, result.y );
    EXPECT_EQ( 9, result.z );

    result += fanVector3<int>( 2, 3, 4 );
    EXPECT_EQ( 7, result.x );
    EXPECT_EQ( 10, result.y );
    EXPECT_EQ( 13, result.z );
}

TEST(fanVector3, scaleMultiplyOperator) {
    fanVector3<int> result = fanVector3<int>( 0, 1, 2 ) * 3;
    EXPECT_EQ( 0, result.x );
    EXPECT_EQ( 3, result.y );
    EXPECT_EQ( 6, result.z );

    result *= 2;
    EXPECT_EQ( 0, result.x );
    EXPECT_EQ( 6, result.y );
    EXPECT_EQ( 12, result.z );
}

TEST(fanVector3, vectorMultiplyOperator) {
    int result = fanVector3<int>( 1, 2, 3 ) * fanVector3<int>( 4, 5, 6 );
    EXPECT_EQ( 32, result );
}

TEST(fanVector3, vectorLength) {
    fanVector3<int> result( 1, 2, 2 );
    EXPECT_EQ( 1, result.x );
    EXPECT_EQ( 2, result.y );
    EXPECT_EQ( 2, result.z );

    EXPECT_EQ( 3, result.length() );
    EXPECT_EQ( 9, result.lengthSquare() );
}

TEST(fanVector3, vectorNormalize) {
    fanVector3<float> result( 1.0f, 2.0f, 2.0f );

    EXPECT_FLOAT_EQ( 3.0f, result.length() );
    result.normalize();
    EXPECT_FLOAT_EQ( 1.0f/3.0f, result.x );
    EXPECT_FLOAT_EQ( 2.0f/3.0f, result.y );
    EXPECT_FLOAT_EQ( 2.0f/3.0f, result.z );
}

TEST(fanVector3, vectorCross) {
    fanVector3<float> x( 1.0f, .0f, .0f );
    fanVector3<float> y( .0f, 1.0f, .0f );

    fanVector3<float> z = cross( x, y );
    EXPECT_FLOAT_EQ( .0f, z.x );
    EXPECT_FLOAT_EQ( .0f, z.y );
    EXPECT_FLOAT_EQ( 1.0f, z.z );
}
