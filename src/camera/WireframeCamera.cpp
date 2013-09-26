#include "WireframeCamera.h"

#include "algo/clip/CohenSutherland.h"
#include "algo/rasterize/line_generator/Bresenham.h"
#include "algo/rasterize/line_generator/DigitalDifferentialAnalyzer.h"

#include "Culling.h"

using namespace fan;

void WireframeCamera::takePicture( fan::fanScene& scene,
                                   fan::fanFilm& film,
                                   fan::fanLens& lens ) {
    fanVector<int, 2> dimens = film.getDimens();
    fanVector<float, 4> window;
    window[0] = 0; window[1] = dimens[0] - 1;
    window[2] = 0; window[3] = dimens[1] - 1;
    Bresenham lineGenerator;

    fanVector<float, 2> p1, p2;
    fanVector<float, 2> a, b, c;
    bool aVisible, bVisible, cVisible;

    fanVector<float, 2> normalPos, triangleCenter;

    fanPixel pixel( 255, 255, 0, 0 );
    fanPixel normalVisible( 255, 0, 255, 0 );
    fanPixel normalInvisible( 255, 0, 0, 255 );

    for ( auto itor = scene.mTriangles.begin(), end = scene.mTriangles.end();
            itor != end; ++itor ) {

#define PLOT_LINE( P1, P2, WINDOW, PIXEL, FILM )                \
        if ( CohenSutherland( WINDOW, P1, P2 ) ) {              \
            lineGenerator.plotLine( P1, P2, PIXEL, FILM );      \
        }

        if ( !Culling( lens, itor->mNormal ) ) {
            continue;
        }

        aVisible = project( *(itor->a), lens, dimens, a );
        bVisible = project( *(itor->b), lens, dimens, b );
        cVisible = project( *(itor->c), lens, dimens, c );
        if ( !aVisible && !bVisible && !cVisible ) {
            continue;
        }

        p1 = a; p2 = b;
        PLOT_LINE( p1, p2, window, pixel, film );

        p1 = b; p2 = c;
        PLOT_LINE( p1, p2, window, pixel, film );

        p1 = c; p2 = a;
        PLOT_LINE( p1, p2, window, pixel, film );
    }
}

