#ifndef RASTERISATIONCAMERA_H

#define RASTERISATIONCAMERA_H

#include <fanCamera.h>
#include <algo/rasterize/fanScanLineGenerator.h>

template<typename CompaionDataType>
class RasterisationCamera
    : public fan::fanCamera
{
public:
    virtual ~RasterisationCamera() {}

    virtual void begin( fan::fanScene& scene,
                        fan::fanFilm& film,
                        fan::fanLens& lens) = 0;
    virtual void end() = 0;
    virtual void nextTriangle( fan::fanTriangleMesh& object,
                               fan::fanTriangle& triangle ) = 0;
    virtual void getCompaionData( size_t i,
                                  fan::fanTriangle& triangle,
                                  fan::fanVector<float,4>& coord,
                                  fan::fanTriangleMesh& object,
                                  CompaionDataType& data ) = 0;
    virtual void plot( fan::fanVector<float, 2> pos,
                       CompaionDataType& data,
                       fan::fanFilm& film ) = 0;

    virtual void takePicture( fan::fanScene& scene,
                              fan::fanFilm& film,
                              fan::fanLens& lens ) {
        fan::fanVector<int, 2> dimens = film.getDimens();

        fan::fanVector<float, 2> a, b, c;
        fan::fanVector<float, 4> homoA, homoB, homoC;
        CompaionDataType compA, compB, compC;
        bool aVisible, bVisible, cVisible;

        fan::fanScanLineGenerator<CompaionDataType> scanLine( dimens );

        begin( scene, film, lens );

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
                    nextTriangle( **object, *itor );

                    scanLine.reset();
                    getCompaionData( 0, *itor, homoA, **object, compA );
                    getCompaionData( 1, *itor, homoB, **object, compB );
                    getCompaionData( 2, *itor, homoC, **object, compC );

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
                        int left, right;
                        CompaionDataType valueAtLeft, valueAtRight;
                        if ( scanLine.mLines[i] == 2 ) {
                            left = scanLine.mXLeft[i];
                            right = scanLine.mXRight[i];
                            valueAtLeft = scanLine.mLeft[i];
                            valueAtRight = scanLine.mRight[i];
                        } else {
                            right = left = scanLine.mXLeft[i];
                            valueAtRight= valueAtLeft = scanLine.mLeft[i];
                        }
                        CompaionDataType Step = (left==right)?
                                                    CompaionDataType():
                                                    (valueAtRight-valueAtLeft)/
                                                    (right-left);
                        CompaionDataType Value = valueAtLeft;
                        for ( int j = left, max = right;
                                j <= max; ++j, Value+=Step ) {
                            if ( j < 0 || j >= dimens[0] ) continue;
                            a[0] = j;
                            plot( a, Value, film );
                        }
                    }
                }
            }
        }
        end();
    }
};

#endif /* end of include guard: RASTERISATIONCAMERA_H */
