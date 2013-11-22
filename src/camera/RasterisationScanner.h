#ifndef RASTERISATIONCAMERA_H

#define RASTERISATIONCAMERA_H

#include <fanScanner.h>
#include <algo/rasterize/fanScanLineGenerator.h>

#include <texture/MemoryTexture.h>

#include <omp.h>

template<typename FillerType, typename ValueType>
class RasterisationScanner
    : public fan::fanScanner<int, ValueType, 2>
{
public:
    RasterisationScanner( FillerType& filler )
        : mFiller( filler )
    {
    }
    virtual ~RasterisationScanner() {};

    class Data {
    public:
        Data()
            : depth( 0 )
            , mData() {}

        Data operator-( const Data& o ) const {
            Data result = *this;
            result.depth -= o.depth;
            result.mData -= o.mData;
            return result;
        }

        Data operator*( const int& ratio ) const {
            Data result = *this;
            result.depth *= ratio;
            result.mData *= ratio;
            return result;
        }

        Data operator/( const int& ratio ) const {
            Data result = *this;
            result.depth /= ratio;
            result.mData /= ratio;
            return result;
        }

        Data& operator+=( const Data& o ) {
            this->depth += o.depth;
            this->mData += o.mData;
            return *this;
        }

        float depth;
        typename FillerType::Data mData;
    };

    virtual void takePicture( fan::fanScene& scene,
                              fan::fanTexture<int, ValueType, 2>& texture,
                              fan::fanLens& lens ) {
        fan::fanVector<int, 2> dimens = texture.getDimens();

        fan::fanVector<float, 2> a, b, c;
        fan::fanVector<float, 4> homoA, homoB, homoC;
        Data compA, compB, compC;
        bool aVisible, bVisible, cVisible;

        FillerType filler = mFiller;

        filler.begin( scene, texture, lens );

        int left, right;
        Data valueAtLeft, valueAtRight, Step, Value;

        fan::fanScanLineGenerator<Data> scanLine( dimens );

        for ( auto object = scene.mTriangleMeshObjects.begin(),
                objEnd = scene.mTriangleMeshObjects.end();
                object != objEnd; ++object ) {

            for ( auto mesh = (*object)->mMeshes.begin(),
                       end = (*object)->mMeshes.end();
                  mesh != end; ++mesh ) {

                fan::fanTriangle* itor = NULL;
                #pragma omp parallel for private(itor,a,b,c,\
                                                 homoA,homoB,homoC,\
                                                 compA,compB,compC,\
                                                 aVisible,bVisible,cVisible,\
                                                 left,right,\
                                                 valueAtLeft,valueAtRight,\
                                                 Step,Value)\
                                                 firstprivate(scanLine,filler)
                for ( size_t i = 0;
                        i < (*mesh)->mFaces->mSize; ++i )
                {

                    itor = (*mesh)->mFaces->mBuffer + i;

                    if ( !lens.cullFace( *itor, (*object)->mObjectToWorld ) ) {
                        continue;
                    }

                    project( transform( (*object)->mObjectToWorld,
                                            *(itor->points[0]) ),
                                        lens, dimens,
                                        a, homoA );
                    project( transform( (*object)->mObjectToWorld,
                                            *(itor->points[1]) ),
                                        lens, dimens,
                                        b, homoB );
                    project( transform( (*object)->mObjectToWorld,
                                            *(itor->points[2]) ),
                                        lens, dimens,
                                        c, homoC );
                    aVisible = !(homoA[0] < 0 || homoA[0] > 1
                               || homoA[1] < 0 || homoA[1] > 1);
                    bVisible = !(homoB[0] < 0 || homoB[0] > 1
                               || homoB[1] < 0 || homoB[1] > 1);
                    cVisible = !(homoC[0] < 0 || homoC[0] > 1
                               || homoC[1] < 0 || homoC[1] > 1);

                    if ( !aVisible && !bVisible && !cVisible ) {
                        continue;
                    }
                    filler.nextTriangle( **object, **mesh, *itor );

                    scanLine.reset();
                    compA.depth = homoA[2];
                    compB.depth = homoB[2];
                    compC.depth = homoC[2];

                    filler.getCompaionData( 0, *itor, **mesh, **object, homoA, compA.mData );
                    filler.getCompaionData( 1, *itor, **mesh, **object, homoB, compB.mData );
                    filler.getCompaionData( 2, *itor, **mesh, **object, homoC, compC.mData );

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
                            filler.plot( a, Value.mData, Value.depth, texture );
                        }
                    }
                }
            }
        }
        filler.end();
    }

    FillerType& mFiller;
};

#endif /* end of include guard: RASTERISATIONCAMERA_H */
