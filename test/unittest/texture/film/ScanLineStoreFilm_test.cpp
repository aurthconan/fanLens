#include <gtest/gtest.h>

#include <texture/film/ScanLineStoreFilm.h>
#include <texture/film/FreeImageFilm.h>
#include <algo/rasterize/line_generator/Bresenham.h>

using namespace fan;

TEST(ScanLineStoreFilm,Triangle) {
    fanVector<int, 2> dimens;
    dimens[0] = 200; dimens[1] = 200;
    ScanLineStoreFilm scanLine( dimens );
    FreeImageFilm freeImage( dimens, "ScanLineStoreFilm_triangle.png" );

    Bresenham lineGenerator;
    fanVector<float, 2> p1, p2;

    p1[0] = 10; p1[1] = 10;
    p2 = p1; p2[0] = 190;
    lineGenerator.plotLine(p1, p2, scanLine );

    p2[1] = 190;
    lineGenerator.plotLine(p1, p2, scanLine );

    p1[0] = 190;
    lineGenerator.plotLine(p1, p2, scanLine );

    EXPECT_EQ( scanLine.mYMin, 10 );
    EXPECT_EQ( scanLine.mYMax, 190 );

    fanPixel pixel(255, 255, 0, 0);
    fanVector<int, 2> pos;                                        \

#define PLOT_SCANLINE                                             \
    for ( int i = scanLine.mYMin; i <= scanLine.mYMax; ++i ) {    \
        fanVector<int, 2> line = scanLine.mpYBucket[i];           \
        pos[1] = i;                                               \
        for ( int j = line[0]; j <= line[1]; ++j ) {              \
            pos[0] = j;                                           \
            freeImage.setPixel( pos, pixel );                     \
        }                                                         \
    }

    PLOT_SCANLINE;

    scanLine.reset();
    p1[0] = 100; p1[1] = 100;
    p2 = p1; p2[0] = -90;
    lineGenerator.plotLine(p1, p2, scanLine );

    p2 = p1; p2[1] = 290;
    lineGenerator.plotLine(p1, p2, scanLine );

    p1[0] = -90;
    lineGenerator.plotLine(p1, p2, scanLine );

    EXPECT_EQ( scanLine.mYMin, 100 );
    EXPECT_EQ( scanLine.mYMax, 199 );

    pixel.r = 0;
    pixel.g = 255;
    PLOT_SCANLINE;

    scanLine.reset();

    p1[0] = 100; p1[1] = 100;
    p2 = p1; p2[0] = 260;
    lineGenerator.plotLine(p1, p2, scanLine );

    p2 = p1; p2[1] = -50;
    lineGenerator.plotLine(p1, p2, scanLine );

    p1[0] = 260;
    lineGenerator.plotLine(p1, p2, scanLine );

    EXPECT_EQ( scanLine.mYMin, 0 );
    EXPECT_EQ( scanLine.mYMax, 100 );

    pixel.g = 0;
    pixel.b = 255;
    PLOT_SCANLINE;

    freeImage.develope();
}

