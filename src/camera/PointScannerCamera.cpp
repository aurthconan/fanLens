#include "PointScannerCamera.h"

#if ENABLE_OPENMP
#include <omp.h>
#endif  // ENABLE_OPENMP

using namespace fan;

void PointScannerCamera::takePicture( fanScene& scene,
                                      fanTexture<int, fanPixel, 2>& film,
                                      fanLens& lens ) {
    fanVector<float, 2> pos;
    fanVector<int, 2> dimens = film.getDimens();
    fanVector<float, 4> homoPos;

    for ( auto object = scene.mTriangleMeshObjects.begin(),
            objEnd = scene.mTriangleMeshObjects.end();
            object != objEnd; ++object ) {
        for ( auto points = (*object)->mMeshes.begin(),
                   end = (*object)->mMeshes.end();
                   points != end; ++points ) {

            /*
            for ( auto itor = (*points)->mVertices->mBuffer,
                    end = (*points)->mVertices->mBuffer
                            + (*points)->mVertices->mSize;
                    itor != end; ++itor ) {
                    */

#if ENABLE_OPENMP
#pragma omp parallel for private(pos, homoPos)
#endif  // ENABLE_OPENMP
            for ( size_t j = 0; j < (*points)->mVertices->mSize; ++j )
            {
                if ( !project( transform( (*object)->mObjectToWorld, *((*points)->mVertices->mBuffer+j) ),
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

