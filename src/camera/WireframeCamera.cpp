#include "WireframeCamera.h"

#include "algo/clip/CohenSutherland.h"
#include "algo/rasterize/line_generator/Bresenham.h"
#include "algo/rasterize/line_generator/DigitalDifferentialAnalyzer.h"

#include <texture/MemoryTexture.h>
#include <texture/ClampTexture.h>

#include "Culling.h"

using namespace fan;

class TextureWithZBufferTest
    : public fanFilm
    , public fanLineGeneratorCallback
{
public:
    TextureWithZBufferTest( fanTexture<int, fanPixel, 2>& film,
                            MemoryTexture<int, float, 2>& zBuffer )
        : fanFilm( film.getDimens() )
        , mFilm( film )
        , mZBuffer( zBuffer )
        , mStep(0)
        , mStart(0)
        , mRange(0) {
    }

    void setValue( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel ) {
        if ( mZBuffer.getValue( index ) > mStart ) {
            mFilm.setValue( index, pixel );
        }
        mStart += mStep;
    }

    void setStep( size_t step ) {
        mStep = mRange / (float) step;
    }

    fanTexture<int, fanPixel, 2>& mFilm;
    MemoryTexture<int, float, 2>& mZBuffer;
    float mStep;
    float mStart;
    float mRange;
};

void WireframeCamera::takePicture( fan::fanScene& scene,
                                   fan::fanFilm& film,
                                   fan::fanLens& lens ) {
    fanVector<int, 2> dimens = film.getDimens();
    fanVector<float, 4> window;
    window[0] = 0; window[1] = dimens[0] - 1;
    window[2] = 0; window[3] = dimens[1] - 1;
    Bresenham lineGenerator;

    fanVector<float, 2> a, b, c;
    fanVector<float, 4> homoA, homoB, homoC;
    bool aVisible, bVisible, cVisible;

    fanPixel pixel( 255, 255, 0, 0 );

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );

    ClampTexture<int, fanPixel, 2> clampFilm( &film );

    TextureWithZBufferTest zBufferTestFilm( clampFilm, zBuffer );

    for ( auto itor = scene.mTriangles.begin(), end = scene.mTriangles.end();
            itor != end; ++itor ) {

#define PLOT_LINE( P1, P2, WINDOW, PIXEL, FILM )                \
        lineGenerator.plotLine( P1, P2, PIXEL, FILM, NULL );    \

        if ( !Culling( lens, itor->mNormal ) ) {
            continue;
        }

        aVisible = project( *(itor->a), lens, dimens, a, homoA );
        bVisible = project( *(itor->b), lens, dimens, b, homoB );
        cVisible = project( *(itor->c), lens, dimens, c, homoC );
        if ( !aVisible && !bVisible && !cVisible ) {
            continue;
        }

        PLOT_LINE( a, b, window, pixel, zBufferTestFilm );

        PLOT_LINE( b, c, window, pixel, zBufferTestFilm );

        PLOT_LINE( c, a, window, pixel, zBufferTestFilm );
    }
}

