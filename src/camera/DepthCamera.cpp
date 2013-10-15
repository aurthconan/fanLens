#include "DepthCamera.h"

#include <algo/rasterize/line_generator/Bresenham.h>

#include <texture/MemoryTexture.h>
#include <texture/ScanLineStoreTexture.h>

#include <utils/SortTriangleByZDepth.h>

using namespace fan;

class ZDepthComputeTexture
    : public fanFilm
    , public fanLineGeneratorCallback
{
public:
    ZDepthComputeTexture( fanVector<int, 2>& dimens,
                          ScanLineStoreTexture<float>& scanLine )
        : fanFilm( dimens )
        , mScanLineStore( scanLine )
        , mStep(0)
        , mStart(0)
        , mRange(0) {
    }

    void setValue( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel ) {
        (void) pixel;
        mScanLineStore.setValue( index, mStart );
        mStart += mStep;
    }

    void setStep( size_t step ) {
        mStep = mRange / (float) step;
    }

    ScanLineStoreTexture<float>& mScanLineStore;
    float mStep;
    float mStart;
    float mRange;
};

void DepthCamera::takePicture( fan::fanScene& scene,
                               fan::fanFilm& film,
                               fan::fanLens& lens ) {
    fanVector<int, 2> dimens = film.getDimens();
    Bresenham lineGenerator;

    fanVector<float, 2> a, b, c;
    fanVector<float, 4> homoA, homoB, homoC;
    bool aVisible, bVisible, cVisible;

    fanPixel pixel( 255, 255, 0, 0 );

    ScanLineStoreTexture<float> scanLine(dimens);
    ZDepthComputeTexture texture( dimens, scanLine );

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );


    for ( auto itor = scene.mTriangles.begin(), end = scene.mTriangles.end();
            itor != end; ++itor ) {

        if ( !lens.cullFace( *itor ) ) {
            continue;
        }

#define PLOT_LINE( P1, P2, START, END, PIXEL, FILM )                \
        if ( P1[0] < P2[0] ) {                                      \
            FILM.mStart = START;                                    \
            FILM.mRange = END - START;                              \
            lineGenerator.plotLine( P1, P2, PIXEL, FILM, &FILM );   \
        } else {                                                    \
            FILM.mStart = END;                                      \
            FILM.mRange = START - END;                              \
            lineGenerator.plotLine( P2, P1, PIXEL, FILM, &FILM );   \
        }

        aVisible = project( *(itor->a), lens, dimens, a, homoA );
        bVisible = project( *(itor->b), lens, dimens, b, homoB );
        cVisible = project( *(itor->c), lens, dimens, c, homoC );
        if ( !aVisible && !bVisible && !cVisible ) {
            continue;
        }
        scanLine.reset();

        PLOT_LINE( a, b, homoA[2], homoB[2],
                     pixel, texture );

        PLOT_LINE( b, c, homoB[2], homoC[2],
                     pixel, texture );

        PLOT_LINE( c, a, homoC[2], homoA[2],
                     pixel, texture );

        // fill the zBuffer
        for ( int i = scanLine.mYMin; i <= scanLine.mYMax; ++i ) {
            auto line = scanLine.mYBucket[i];
            if ( line.xMin >= dimens[0] ) continue;
            if ( line.xMax < 0 ) continue;
            a[1] = i;
            float depthStep = (line.xMin==line.xMax)?0:
                                    (line.valueAtMax-line.valueAtMin)/
                                        (line.xMax-line.xMin);
            float depthValue = line.valueAtMin;
            for ( int j = line.xMin, max = line.xMax;
                    j <= max; ++j, depthValue+=depthStep ) {
                if ( j < 0 || j >= dimens[0] ) continue;
                a[0] = j;
                if ( depthValue < zBuffer.getValue( a ) ) {
                    zBuffer.setValue( a, depthValue );

                    pixel.r = ( 1.0f - depthValue ) * 255.0f;
                    pixel.g = ( 1.0f - depthValue ) * 255.0f;
                    pixel.b = ( 1.0f - depthValue ) * 255.0f;
                    film.setValue( a, pixel );
                }
            }
        }
    }
}

