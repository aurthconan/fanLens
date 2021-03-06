#include <gtest/gtest.h>

#include <texture/ScanLineStoreTexture.h>
#include <texture/film/FreeImageFilm.h>
#include <algo/rasterize/line_generator/Bresenham.h>
#include <fanPixel.h>

using namespace fan;

class ScanLineStoreTextureTestStub
    : public fanTexture<int, fanPixel, 2>
{
public:
    ScanLineStoreTextureTestStub( const fanVector<int, 2>& dimensions )
        : fanTexture<int, fanPixel, 2>( dimensions ) {
    }
    virtual void setValue( const fanVector<int, 2>& index,
                           const fanPixel& value ) {
        (void) value;
        texture->setValue( index, .0f );
    }

    ScanLineStoreTexture<fanPixel>* texture;
};

TEST(ScanLineStoreTexture,Triangle) {
    fanVector<int, 2> dimens;
    fanPixel pixel(255, 255, 0, 0);
    dimens[0] = 200; dimens[1] = 200;
    ScanLineStoreTexture<fanPixel> scanLine( dimens );
    ScanLineStoreTextureTestStub stub( dimens );
    stub.texture = &scanLine;
    FreeImageFilm freeImage( dimens, "ScanLineStoreFilm_triangle.png" );

    Bresenham lineGenerator;
    fanVector<float, 2> p1, p2;

    p1[0] = 10; p1[1] = 10;
    p2 = p1; p2[0] = 190;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    p2[1] = 190;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    p1[0] = 190;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    EXPECT_EQ( scanLine.mYMin, 10 );
    EXPECT_EQ( scanLine.mYMax, 190 );

    fanVector<int, 2> pos;

#define PLOT_SCANLINE                                               \
    for ( int i = scanLine.mYMin; i <= scanLine.mYMax; ++i ) {      \
        auto line = scanLine.mYBucket[i];                           \
        if ( line.xMin >= dimens[0] ) continue;                     \
        if ( line.xMax < 0 ) continue;                              \
        pos[1] = i;                                                 \
        for ( int j = line.xMin, max = line.xMax;                   \
                j < max; ++j ) {                                    \
            if ( j < 0 || j >= dimens[0] ) continue;                \
            pos[0] = j;                                             \
            freeImage.setValue( pos, pixel );                       \
        }                                                           \
    }

    PLOT_SCANLINE;

    scanLine.reset();
    p1[0] = 100; p1[1] = 100;
    p2 = p1; p2[0] = -90;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    p2 = p1; p2[1] = 290;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    p1[0] = -90;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    EXPECT_EQ( scanLine.mYMin, 100 );
    EXPECT_EQ( scanLine.mYMax, 199 );

    pixel.r = 0;
    pixel.g = 255;
    PLOT_SCANLINE;

    scanLine.reset();

    p1[0] = 100; p1[1] = 100;
    p2 = p1; p2[0] = 260;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    p2 = p1; p2[1] = -50;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    p1[0] = 260;
    lineGenerator.plotLine(p1, p2, pixel, stub, NULL );

    EXPECT_EQ( scanLine.mYMin, 0 );
    EXPECT_EQ( scanLine.mYMax, 100 );

    pixel.g = 0;
    pixel.b = 255;
    PLOT_SCANLINE;

    freeImage.develope();
}

