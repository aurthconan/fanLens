#include "DepthCamera.h"

#include "Culling.h"

#include <algo/rasterize/line_generator/Bresenham.h>

#include <texture/MemoryTexture.h>
#include <texture/ScanLineStoreTexture.h>

#include <utils/SortTriangleByZDepth.h>

#include <algorithm>

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

    // sort the depth
    std::sort(scene.mTriangles.begin(), scene.mTriangles.end(),
                    SortTriangleByZDepth(lens));

    ScanLineStoreTexture<float> scanLine(dimens);
    ZDepthComputeTexture texture( dimens, scanLine );

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );


    for ( auto itor = scene.mTriangles.begin(), end = scene.mTriangles.end();
            itor != end; ++itor ) {

        if ( !Culling( lens, itor->mNormal ) ) {
            continue;
        }

#define PLOT_LINE( P1, P2, START, RANGE, PIXEL, FILM )      \
        FILM.mStart = START;                                        \
        FILM.mRange = RANGE;                                        \
        lineGenerator.plotLine( P1, P2, PIXEL, FILM, &FILM );       \

        aVisible = project( *(itor->a), lens, dimens, a, homoA );
        bVisible = project( *(itor->b), lens, dimens, b, homoB );
        cVisible = project( *(itor->c), lens, dimens, c, homoC );
        if ( !aVisible && !bVisible && !cVisible ) {
            continue;
        }
        scanLine.reset();

        PLOT_LINE( a, b, homoA[2], homoB[2] - homoA[2],
                     pixel, texture );

        PLOT_LINE( b, c, homoB[2], homoC[2] - homoB[2],
                     pixel, texture );

        PLOT_LINE( c, a, homoC[2], homoA[2] - homoC[2],
                     pixel, texture );

        fanPixel pixel( 255, 255, 255, 255 );

        // fill the zBuffer
        for ( int i = scanLine.mYMin; i <= scanLine.mYMax; ++i ) {
            auto line = scanLine.mYBucket[i];
            if ( line.first.first >= dimens[0] ) continue;
            if ( line.second.first < 0 ) continue;
            a[1] = i;
            float depthStep = (line.first.first==line.second.first)?0:
                                    (line.second.second-line.first.second)/
                                        (line.second.first-line.first.first);
            float depthValue = line.first.second;
            for ( int j = line.first.first, max = line.second.first;
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

