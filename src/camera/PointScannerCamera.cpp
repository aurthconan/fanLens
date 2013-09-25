#include "PointScannerCamera.h"

using namespace fan;

void PointScannerCamera::takePicture( fanScene& scene,
                                      fanFilm& film,
                                      fanLens& lens ) {
    fanVector<float, 2> pos;
    fanVector<int, 2> dimens = film.getDimens();

    for ( auto itor = scene.mVertices.begin(), end = scene.mVertices.end();
            itor != end; ++itor ) {

        if ( !project( *itor, lens, dimens, pos ) ) {
            continue;
        }

        // hard code the color temporary
        fanPixel pixel( 255, 255, 0, 0 );
        film.setPixel( pos, pixel );
    }
}

