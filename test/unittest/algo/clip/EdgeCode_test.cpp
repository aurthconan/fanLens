#include <gtest/gtest.h>

#include <algo/clip/EdgeCode.h>

using namespace fan;
/*     |            |
 * 1001|   1000     |1010
 * ----------------------
 *     |            |
 * 0001|   0000     |0010
 * ----------------------
 *     |            |
 * 0101|   0100     |0110
 */

TEST(EdgeCode_test,all) {
    fanVector<float, 4> window;
    window[0] = 0;
    window[1] = 100;
    window[2] = 0;
    window[3] = 100;

    fanVector<float, 2> dot;
    dot[0] = -1;
    dot[1] = 101;

    EXPECT_EQ( 9, EncodeDot( dot, window ) ); // (1001)2 = (9)10

    dot[0] = 0;
    EXPECT_EQ( 8, EncodeDot( dot, window ) );

    dot[0] = 101;
    EXPECT_EQ( 10, EncodeDot( dot, window ) );

    dot[1] = 100;
    EXPECT_EQ( 2, EncodeDot( dot, window ) );

    dot[1] = -1;
    EXPECT_EQ( 6, EncodeDot( dot, window ) );

    dot[0] = 0;
    EXPECT_EQ( 4, EncodeDot( dot, window ) );

    dot[0] = -1;
    EXPECT_EQ( 5, EncodeDot( dot, window ) );

    dot[1] = 0;
    EXPECT_EQ( 1, EncodeDot( dot, window ) );

    dot[0] = -100;
    dot[1] = 50;
    EXPECT_EQ( 1, EncodeDot( dot, window ) );

}

