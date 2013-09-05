#include "WireframeCamera.h"

using namespace fan;

void PlotLine( fanFilm& film,
               fanVector<int, 2> begin, fanVector<int, 2> end ) {

}

void WireframeCamera::takePicture( fan::fanScene& scene,
                                   fan::fanFilm& film,
                                   fan::fanLens& lens ) {
    for ( auto itor = scene.mTriangles.begin(), end = scene.mTriangles.end();
            itor != end; ++itor ) {
        fanVector<int, 2> a, b, c;
        if ( !lens.project( a, itor->a, film )
            && !lens.project( b, itor->b, film )
            && !lens.project( c, itor->c, film ) ) {
            continue;
        }

        PlotLine( film, a, b );
        PlotLine( film, b, c );
        PlotLine( film, c, a );
    }
}

