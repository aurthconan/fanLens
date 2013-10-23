#include "FlatShadingCamera.h"

#include <algo/rasterize/fanScanLineGenerator.h>
#include <texture/MemoryTexture.h>
#include <lights/fanLightsAccumulator.h>

using namespace fan;

void FlatShadingCamera::takePicture( fan::fanScene& scene,
                                     fan::fanFilm& film,
                                     fan::fanLens& lens ) {
    fanVector<int, 2> dimens = film.getDimens();

    fanVector<float, 2> a, b, c;
    fanVector<float, 4> homoA, homoB, homoC;
    bool aVisible, bVisible, cVisible;

    fanPixel pixel( 255, 0, 0, 0 );

    fanScanLineGenerator<float> scanLine( dimens );

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );

    fanLightsAccumulator lightAccumulator( scene.mLights );

    for ( auto object = scene.mTriangleMeshes.begin(),
            objEnd = scene.mTriangleMeshes.end();
            object != objEnd; ++object ) {

        for ( auto mesh = (*object)->mFaces.begin(),
                   end = (*object)->mFaces.end();
              mesh != end; ++mesh ) {

            for ( auto itor = (*mesh)->mBuffer,
                    end = (*mesh)->mBuffer + (*mesh)->mSize;
                    itor != end; ++itor ) {

                if ( !lens.cullFace( *itor, (*object)->mObjectToWorld ) ) {
                    continue;
                }

                pixel = lightAccumulator.getLight( itor->mCenter, itor->mNormal, lens.mPos );

                aVisible = project( *(itor->a), lens, (*object)->mObjectToWorld, dimens,
                                    a, homoA );
                bVisible = project( *(itor->b), lens, (*object)->mObjectToWorld, dimens,
                                    b, homoB );
                cVisible = project( *(itor->c), lens, (*object)->mObjectToWorld, dimens,
                                    c, homoC );
                if ( !aVisible && !bVisible && !cVisible ) {
                    continue;
                }
                scanLine.reset();

                scanLine.AddLine( a, b, homoA[2], homoB[2] );
                scanLine.AddLine( b, c, homoB[2], homoC[2] );
                scanLine.AddLine( c, a, homoC[2], homoA[2] );

                // fill the zBuffer
                for ( int i = scanLine.mYMin; i <= scanLine.mYMax; ++i ) {
                    if ( scanLine.mLines[i] == 0 ) {
                        continue;
                    } else if ( scanLine.mLines[i] == 1
                                && (scanLine.mXLeft[i] >= dimens[0]
                                || scanLine.mXLeft[i] < 0 ) ) {
                        continue;
                    } else if ( scanLine.mLines[i] == 2
                                && (scanLine.mXLeft[i] >= dimens[0]
                                    || scanLine.mXRight[i] < 0 ) ) {
                        continue;
                    } else if ( scanLine.mLines[i] > 2 ) {
                        continue;
                    }
                    a[1] = i;
                    int left, right;
                    float valueAtLeft, valueAtRight;
                    if ( scanLine.mLines[i] == 2 ) {
                        left = scanLine.mXLeft[i];
                        right = scanLine.mXRight[i];
                        valueAtLeft = scanLine.mLeft[i];
                        valueAtRight = scanLine.mRight[i];
                    } else {
                        right = left = scanLine.mXLeft[i];
                        valueAtRight= valueAtLeft = scanLine.mLeft[i];
                    }
                    float depthStep = (left==right)?0:
                                            (valueAtRight-valueAtLeft)/
                                                (right-left);
                    float depthValue = valueAtLeft;
                    for ( int j = left, max = right;
                            j <= max; ++j, depthValue+=depthStep ) {
                        if ( j < 0 || j >= dimens[0] ) continue;
                        a[0] = j;
                        if ( depthValue < zBuffer.getValue( a ) ) {
                            zBuffer.setValue( a, depthValue );
                            film.setValue( a, pixel );
                        }
                    }
                }
            }
        }
    }
}

