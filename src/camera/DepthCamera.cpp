#include "DepthCamera.h"

#include <algo/rasterize/fanScanLineGenerator.h>

#include <texture/MemoryTexture.h>

using namespace fan;

void DepthCamera::takePicture( fan::fanScene& scene,
                               fan::fanFilm& film,
                               fan::fanLens& lens ) {
    fanVector<int, 2> dimens = film.getDimens();

    fanVector<float, 2> a, b, c;
    fanVector<float, 4> homoA, homoB, homoC;
    bool aVisible, bVisible, cVisible;

    fanPixel pixel( 255, 255, 0, 0 );

    fanScanLineGenerator<float> scanLine( dimens );

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );

    for ( auto object = scene.mTriangleMeshObjects.begin(),
            objEnd = scene.mTriangleMeshObjects.end();
            object != objEnd; ++object ) {

        for ( auto mesh = (*object)->mMeshes.begin(),
                   end = (*object)->mMeshes.end();
              mesh != end; ++mesh ) {

            for ( auto itor = (*mesh)->mFaces->mBuffer,
                    end = (*mesh)->mFaces->mBuffer + (*mesh)->mFaces->mSize;
                    itor != end; ++itor ) {

                if ( !lens.cullFace( *itor, (*object)->mObjectToWorld ) ) {
                    continue;
                }

                aVisible = project( transform( (*object)->mObjectToWorld,
                                        *(itor->points[0]) ),
                                    lens, dimens,
                                    a, homoA );
                bVisible = project( transform( (*object)->mObjectToWorld,
                                        *(itor->points[1]) ),
                                    lens, dimens,
                                    b, homoB );
                cVisible = project( transform( (*object)->mObjectToWorld,
                                        *(itor->points[2]) ),
                                    lens, dimens,
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

                            pixel.r = ( 1.0f - depthValue ) * 255.0f;
                            pixel.g = ( 1.0f - depthValue ) * 255.0f;
                            pixel.b = ( 1.0f - depthValue ) * 255.0f;
                            film.setValue( a, pixel );
                        }
                    }
                }
            }
        }
    }
}

