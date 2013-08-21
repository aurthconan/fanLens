#include <gtest/gtest.h>

#include <fanVector.h>

using namespace fan;

TEST(fanVector,Constructor_int) {
    fanVector<int> _variable;
    EXPECT_EQ( 0, _variable.x );
    EXPECT_EQ( 0, _variable.y );
    EXPECT_EQ( 0, _variable.z );

    fanVector<int> _variable2( 1, 2, 3 );
    EXPECT_EQ( 1, _variable2.x );
    EXPECT_EQ( 2, _variable2.y );
    EXPECT_EQ( 3, _variable2.z );
}

TEST(fanVector,Constructor_float) {
    fanVector<float> _variable;
    EXPECT_FLOAT_EQ( .0f, _variable.x );
    EXPECT_FLOAT_EQ( .0f, _variable.y );
    EXPECT_FLOAT_EQ( .0f, _variable.z );

    fanVector<float> _variable2( .1f, .2f, .3f );
    EXPECT_FLOAT_EQ( .1f, _variable2.x );
    EXPECT_FLOAT_EQ( .2f, _variable2.y );
    EXPECT_FLOAT_EQ( .3f, _variable2.z );
}
