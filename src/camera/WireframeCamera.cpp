#include "WireframeCamera.h"

#include <algo/clip/CohenSutherland.h>
#include <algo/rasterize/line_generator/Bresenham.h>
#include <algo/rasterize/line_generator/DigitalDifferentialAnalyzer.h>

#include <texture/MemoryTexture.h>
#include <texture/ClampTexture.h>
#include <texture/ScanLineStoreTexture.h>

#include <utils/SortTriangleByZDepth.h>

#include <algorithm>

using namespace fan;

class TextureWithZBufferTest
    : public fanFilm
    , public fanLineGeneratorCallback
{
public:
    TextureWithZBufferTest( fanTexture<int, fanPixel, 2>& film,
                            MemoryTexture<int, float, 2>& zBuffer,
                            ScanLineStoreTexture<float>& scanLine )
        : fanFilm( film.getDimens() )
        , mFilm( film )
        , mZBuffer( zBuffer )
        , mScanLineStore( scanLine )
        , mStep(0)
        , mStart(0)
        , mRange(0) {
    }

    void setValue( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel ) {
        if ( index[0] > 0 && index[0] < mDimensions[0]
                && index[1] > 0 && index[1] < mDimensions[1] ) {
            if ( mZBuffer.getValue( index ) > mStart ) {
                mFilm.setValue( index, pixel );
            }
        }

        mScanLineStore.setValue( index, mStart );
        mStart += mStep;
    }

    void setStep( size_t step ) {
        mStep = mRange / (float) step;
    }

    fanTexture<int, fanPixel, 2>& mFilm;
    MemoryTexture<int, float, 2>& mZBuffer;
    ScanLineStoreTexture<float>& mScanLineStore;
    float mStep;
    float mStart;
    float mRange;
};

void WireframeCamera::takePicture( fan::fanScene& scene,
                                   fan::fanFilm& film,
                                   fan::fanLens& lens ) {
    fanVector<int, 2> dimens = film.getDimens();
    Bresenham lineGenerator;

    fanVector<float, 2> a, b, c;
    fanVector<float, 4> homoA, homoB, homoC;
    bool aVisible, bVisible, cVisible;

    fanPixel pixel( 255, 255, 0, 0 );

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );

    ClampTexture<int, fanPixel, 2> clampFilm( &film );

    ScanLineStoreTexture<float> scanLine(dimens);
    TextureWithZBufferTest zBufferTestFilm( clampFilm, zBuffer, scanLine );

    for ( auto object = scene.mTriangleMeshObjects.begin(),
            objEnd = scene.mTriangleMeshObjects.end();
            object != objEnd; ++object ) {

        for ( auto mesh = (*object)->mMeshes.begin(),
                   end = (*object)->mMeshes.end();
              mesh != end; ++mesh ) {

            // sort the depth
            std::sort((*mesh)->mFaces->mBuffer,
                      (*mesh)->mFaces->mBuffer + (*mesh)->mFaces->mSize,
                            SortTriangleByZDepth(lens));

            for ( auto itor = (*mesh)->mFaces->mBuffer,
                    end = (*mesh)->mFaces->mBuffer + (*mesh)->mFaces->mSize;
                    itor != end; ++itor ) {

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

                if ( !lens.cullFace( *itor, (*object)->mObjectToWorld ) ) {
                    continue;
                }

                aVisible = project( transform( (*object)->mObjectToWorld,
                                        *(itor->points[0] ) ),
                                    lens, dimens,
                                    a, homoA );
                bVisible = project( transform( (*object)->mObjectToWorld,
                                        *(itor->points[1] ) ),
                                    lens, dimens,
                                    b, homoB );
                cVisible = project( transform( (*object)->mObjectToWorld,
                                        *(itor->points[2] ) ),
                                    lens, dimens,
                                    c, homoC );
                if ( !aVisible && !bVisible && !cVisible ) {
                    continue;
                }
                scanLine.reset();

                PLOT_LINE( a, b, homoA[2], homoB[2],
                             pixel, zBufferTestFilm );

                PLOT_LINE( b, c, homoB[2], homoC[2],
                             pixel, zBufferTestFilm );

                PLOT_LINE( c, a, homoC[2], homoA[2],
                             pixel, zBufferTestFilm );

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
                        }
                    }
                }
            }
        }
    }
}

