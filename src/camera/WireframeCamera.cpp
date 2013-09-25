#include "WireframeCamera.h"

#include "algo/clip/CohenSutherland.h"
#include "algo/rasterize/line_generator/Bresenham.h"
#include "algo/rasterize/line_generator/DigitalDifferentialAnalyzer.h"

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

    for ( auto itor = scene.mTriangles.begin(), end = scene.mTriangles.end();
            itor != end; ++itor ) {
        bool aVisible = project( *(itor->a), lens, dimens, a );
        bool bVisible = project( *(itor->b), lens, dimens, b );
        bool cVisible = project( *(itor->c), lens, dimens, c );
        if ( !aVisible && !bVisible && !cVisible ) {
            continue;
        }

        p1 = a; p2 = b;
        if ( CohenSutherland( window, p1, p2 ) ) {
            lineGenerator.plotLine( p1, p2, film );
        }
        p1 = b; p2 = c;
        if ( CohenSutherland( window, p1, p2 ) ) {
            lineGenerator.plotLine( p1, p2, film );
        }
        p1 = c; p2 = a;
        if ( CohenSutherland( window, p1, p2 ) ) {
            lineGenerator.plotLine( p1, p2, film );
        }
    }
}

