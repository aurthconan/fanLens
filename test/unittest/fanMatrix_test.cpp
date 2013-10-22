#include <gtest/gtest.h>

#include <fanMatrix.h>

using namespace fan;

TEST(fanMatrix,Constructor) {
    fanMatrix<int, 1, 1> mat1x1;
    EXPECT_EQ( 0, mat1x1[0][0] );

    fanMatrix<int, 1, 2> mat1x2;
    EXPECT_EQ( 0, mat1x2[0][0] );
    EXPECT_EQ( 0, mat1x2[0][1] );

    fanMatrix<int, 2, 1> mat2x1;
    EXPECT_EQ( 0, mat2x1[0][0] );
    EXPECT_EQ( 0, mat2x1[1][0] );

    mat2x1[0][0] = 4;
    mat2x1[1][0] = 5;
}

TEST(fanMatrix,addOperator) {
    fanMatrix<int, 1, 2> mat1x2_a;
    mat1x2_a[0][0] = 0;
    mat1x2_a[0][1] = 1;
    fanMatrix<int, 1, 2> mat1x2_b;
    mat1x2_b[0][0] = 2;
    mat1x2_b[0][1] = 3;

    fanMatrix<int, 1, 2> mat1x2_c = mat1x2_a + mat1x2_b;
    EXPECT_EQ( 2, mat1x2_c[0][0] );
    EXPECT_EQ( 4, mat1x2_c[0][1] );

    mat1x2_c += mat1x2_b;
    EXPECT_EQ( 4, mat1x2_c[0][0] );
    EXPECT_EQ( 7, mat1x2_c[0][1] );
}

TEST(fanMatrix,multiplyOperator) {
    fanMatrix<int, 1, 2> mat1x2_a;
    mat1x2_a[0][0] = 0;
    mat1x2_a[0][1] = 1;
    fanMatrix<int, 2, 1> mat2x1_b;
    mat2x1_b[0][0] = 2;
    mat2x1_b[1][0] = 3;

    fanMatrix<int, 1, 1> mat1x2_c = mat1x2_a * mat2x1_b;
    EXPECT_EQ( 3, mat1x2_c[0][0] );

    mat1x2_c *= 3;
    EXPECT_EQ( 9, mat1x2_c[0][0] );

    fanMatrix<int, 2, 2> mat2x2_d = mat2x1_b * mat1x2_a;
    EXPECT_EQ( 0, mat2x2_d[0][0] );
    EXPECT_EQ( 2, mat2x2_d[0][1] );
    EXPECT_EQ( 0, mat2x2_d[1][0] );
    EXPECT_EQ( 3, mat2x2_d[1][1] );
}

TEST(fanMatrix,transposeOperator) {
    fanMatrix<int, 2, 1> mat2x1_a;
    mat2x1_a[0][0] = 3;
    mat2x1_a[1][0] = 4;

    fanMatrix<int, 1, 2> mat1x2_b = transpose(mat2x1_a);
    EXPECT_EQ( 3, mat1x2_b[0][0] );
    EXPECT_EQ( 4, mat1x2_b[0][1] );
}

TEST(fanMatrix,initializer_list) {
    fanMatrix<int, 2, 1> mat2x1_a{1,2};
    EXPECT_EQ( 1, mat2x1_a[0][0] );
    EXPECT_EQ( 2, mat2x1_a[1][0] );

    fanMatrix<int, 2, 3> mat2x3_a{1,2,3,
                                  4,5,6,};
    EXPECT_EQ( 1, mat2x3_a[0][0] );
    EXPECT_EQ( 2, mat2x3_a[0][1] );
    EXPECT_EQ( 3, mat2x3_a[0][2] );
    EXPECT_EQ( 4, mat2x3_a[1][0] );
    EXPECT_EQ( 5, mat2x3_a[1][1] );
    EXPECT_EQ( 6, mat2x3_a[1][2] );
}
