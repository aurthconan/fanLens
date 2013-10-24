#include "PointScannerCamera.h"

using namespace fan;

void PointScannerCamera::takePicture( fanScene& scene,
                                      fanFilm& film,
                                      fanLens& lens ) {
    fanVector<float, 2> pos;
    fanVector<int, 2> dimens = film.getDimens();
    fanVector<float, 4> homoPos;

    for ( auto object = scene.mTriangleMeshes.begin(),
            objEnd = scene.mTriangleMeshes.end();
            object != objEnd; ++object ) {
        for ( auto points = (*object)->mVertices.begin(),
                   end = (*object)->mVertices.end();
                   points != end; ++points ) {

            for ( auto itor = (*points)->mBuffer,
                    end = (*points)->mBuffer + (*points)->mSize;
                    itor != end; ++itor ) {

                if ( !project( transform( (*object)->mObjectToWorld, *itor ),
                                lens, dimens,
                                pos, homoPos ) ) {
                    continue;
                }

                // hard code the color temporary
                fanPixel pixel( 255, 255, 0, 0 );
                film.setValue( pos, pixel );
            }
        }
    }
}

