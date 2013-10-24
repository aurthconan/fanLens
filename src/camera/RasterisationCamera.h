#ifndef RASTERISATIONCAMERA_H

#define RASTERISATIONCAMERA_H

#include <fanCamera.h>
#include <algo/rasterize/fanScanLineGenerator.h>

template<typename FillerType>
class RasterisationCamera
    : public fan::fanCamera
{
public:
    virtual ~RasterisationCamera() {}

    virtual void takePicture( fan::fanScene& scene,
                              fan::fanFilm& film,
                              fan::fanLens& lens ) {
        fan::fanVector<int, 2> dimens = film.getDimens();

        fan::fanVector<float, 2> a, b, c;
        fan::fanVector<float, 4> homoA, homoB, homoC;
        typename FillerType::Data compA, compB, compC;
        bool aVisible, bVisible, cVisible;

        fan::fanScanLineGenerator<typename FillerType::Data> scanLine( dimens );

        mFiller.begin( scene, film, lens );

        int left, right;
        typename FillerType::Data valueAtLeft, valueAtRight, Step, Value;

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
                    mFiller.nextTriangle( **object, *itor );

                    scanLine.reset();
                    mFiller.getCompaionData( 0, *itor, homoA, **object, compA );
                    mFiller.getCompaionData( 1, *itor, homoB, **object, compB );
                    mFiller.getCompaionData( 2, *itor, homoC, **object, compC );

                    scanLine.AddLine( a, b, compA, compB );
                    scanLine.AddLine( b, c, compB, compC );
                    scanLine.AddLine( c, a, compC, compA );

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
                        if ( scanLine.mLines[i] == 2 ) {
                            left = scanLine.mXLeft[i];
                            right = scanLine.mXRight[i];
                            valueAtLeft = scanLine.mLeft[i];
                            valueAtRight = scanLine.mRight[i];
                        } else {
                            right = left = scanLine.mXLeft[i];
                            valueAtRight= valueAtLeft = scanLine.mLeft[i];
                        }
                        Step = (left==right)?
                                 Value:
                                 (valueAtRight-valueAtLeft)/(right-left);
                        Value = valueAtLeft;
                        for ( int j = left, max = right;
                                j <= max; ++j, Value+=Step ) {
                            if ( j < 0 || j >= dimens[0] ) continue;
                            a[0] = j;
                            mFiller.plot( a, Value, film );
                        }
                    }
                }
            }
        }
        mFiller.end();
    }
    FillerType mFiller;
};

#endif /* end of include guard: RASTERISATIONCAMERA_H */
