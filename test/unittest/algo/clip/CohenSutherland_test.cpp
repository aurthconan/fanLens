#include <gtest/gtest.h>

#include <algo/clip/CohenSutherland.h>
#include <algo/clip/EdgeCode.h>

using namespace fan;

class LineClipTest
    : public testing::Test
{
public:
/*
 * mPoints grid follow the following graph
 *  0   1   2   3   4   5   6
 * 0.   .   .   .   .   .   .
 *          |       |
 * 1.   .   .   .   .   .   .
 *          |       |
 * 2.---.---.---.---.---.---.
 *          |       |
 * 3.   .   .   .   .   .   .
 *          |       |
 * 4.---.---.---.---.---.---.
 *          |       |
 * 5.   .   .   .   .   .   .
 *          |       |
 * 6.   .   .   .   .   .   .
 *
 */

    void SetUp() {
        window[0] = 0; window[1] = 100;
        window[2] = 0; window[3] = 100;
        for ( float y = 200, i = 0; i < 7; y -= 50, i+=1 ) {
            for ( float x = -100, j = 0; j < 7; x += 50, j+=1 ) {
                mPoints[(int)i][(int)j][0] = x;
                mPoints[(int)i][(int)j][1] = y;
            }
        }
    }

    fanVector<float, 2> mPoints[7][7];
    fanVector<float, 4> window;

    fanVector<float, 2> p1;
    fanVector<float, 2> p2;
};

TEST_F(LineClipTest,CohenSutherland) {
    p1 = mPoints[0][0];
    p2 = mPoints[6][0];
    EXPECT_FALSE(CohenSutherland(window, p1, p2));

    p1 = mPoints[0][0];
    p2 = mPoints[0][6];
    EXPECT_FALSE(CohenSutherland(window, p1, p2));

    p1 = mPoints[0][6];
    p2 = mPoints[6][6];
    EXPECT_FALSE(CohenSutherland(window, p1, p2));

    p1 = mPoints[6][6];
    p2 = mPoints[6][0];
    EXPECT_FALSE(CohenSutherland(window, p1, p2));

    p1 = mPoints[3][0];
    p2 = mPoints[3][6];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(0,p1[0]);
    EXPECT_FLOAT_EQ(50,p1[1]);
    EXPECT_FLOAT_EQ(100,p2[0]);
    EXPECT_FLOAT_EQ(50,p2[1]);

    p1 = mPoints[0][3];
    p2 = mPoints[6][3];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(50,p1[0]);
    EXPECT_FLOAT_EQ(100,p1[1]);
    EXPECT_FLOAT_EQ(50,p2[0]);
    EXPECT_FLOAT_EQ(0,p2[1]);

    p1 = mPoints[0][0];
    p2 = mPoints[6][6];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(0,p1[0]);
    EXPECT_FLOAT_EQ(100,p1[1]);
    EXPECT_FLOAT_EQ(100,p2[0]);
    EXPECT_FLOAT_EQ(0,p2[1]);

    p1 = mPoints[2][1];
    p2 = mPoints[3][3];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(0,p1[0]);
    EXPECT_FLOAT_EQ(75,p1[1]);
    EXPECT_FLOAT_EQ(50,p2[0]);
    EXPECT_FLOAT_EQ(50,p2[1]);

    p1 = mPoints[5][4];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(75,p1[0]);
    EXPECT_FLOAT_EQ(0,p1[1]);
    EXPECT_FLOAT_EQ(50,p2[0]);
    EXPECT_FLOAT_EQ(50,p2[1]);

    p1 = mPoints[4][5];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(100,p1[0]);
    EXPECT_FLOAT_EQ(25,p1[1]);
    EXPECT_FLOAT_EQ(50,p2[0]);
    EXPECT_FLOAT_EQ(50,p2[1]);

    p1 = mPoints[1][4];
    EXPECT_TRUE(CohenSutherland(window, p1, p2));
    EXPECT_FLOAT_EQ(75,p1[0]);
    EXPECT_FLOAT_EQ(100,p1[1]);
    EXPECT_FLOAT_EQ(50,p2[0]);
    EXPECT_FLOAT_EQ(50,p2[1]);
}

