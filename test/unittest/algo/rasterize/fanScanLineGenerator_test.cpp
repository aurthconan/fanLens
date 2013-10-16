#include <gtest/gtest.h>

#include <algo/rasterize/fanScanLineGenerator.h>
#include <fanVector.h>
#include <texture/film/FreeImageFilm.h>

using namespace fan;

TEST(fanScanLineGeneratorTest, fillTriangleCase1) {
    fanVector<int, 2> dimens;
    dimens[0] = 800; dimens[1] = 600;

    fanScanLineGenerator<float> scanLine( dimens );
    fanVector<float, 2> a, b, c;
    a[0] = 131.392; a[1] = 258.553;
    b[0] = 476.121; b[1] = 18.142;
    c[0] = 668.607; c[1] = 341.446;

    scanLine.AddLine( a, b, 0, 0 );
    scanLine.AddLine( b, c, 0, 0 );
    scanLine.AddLine( c, a, 0, 0 );

    FreeImageFilm freeImage( dimens, "fanScanLineGeneratorTest_test_fillTriangleCase1.png" );

    fanVector<int, 2> pos;
    fanPixel pixel( 255, 255, 0, 0 );

    for ( int i = scanLine.mYMin; i <= scanLine.mYMax; ++i ) {
        pos[1] = i;
        int left, right;
        float valueAtLeft, valueAtRight;
        if ( scanLine.mLines[i] == 2 ) {
            left = scanLine.mXLeft[i];
            right = scanLine.mXRight[i];
            valueAtLeft = scanLine.mLeft[i];
            valueAtRight = scanLine.mRight[i];
        } else {
            right = left = scanLine.mXLeft[i];
            valueAtRight= valueAtLeft = scanLine.mLeft[i];
        }

        for ( int j = left, max = right;
                j <= max; ++j ) {
            if ( j < 0 || j >= dimens[0] ) continue;
            pos[0] = j;
            freeImage.setValue( pos, pixel );
        }
    }
    freeImage.develope();
}

