#ifndef RASTERISATIONCAMERA_H

#define RASTERISATIONCAMERA_H

#include <fanCamera.h>
#include <algo/rasterize/fanScanLineGenerator.h>

#include <texture/MemoryTexture.h>

#if ENABLE_OPENMP
#include <omp.h>
#endif  // ENABLE_OPENMP

template<typename FillerType>
class RasterisationCamera
    : public fan::fanCamera
{
public:
    virtual ~RasterisationCamera() {}

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
                              fan::fanTexture<int, fan::fanPixel, 2>& film,
                              fan::fanLens& lens ) {
        fan::fanVector<int, 2> dimens = film.getDimens();

        fan::fanVector<float, 2> a, b, c;
        fan::fanVector<float, 4> homoA, homoB, homoC;
        Data compA, compB, compC;
        bool aVisible, bVisible, cVisible;

        MemoryTexture<int, float, 2> zBuffer( dimens );
        zBuffer.reset( 2.0f );
        FillerType mFiller;

        mFiller.begin( scene, film, lens );

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
#if ENABLE_OPENMP
                #pragma omp parallel for private(itor,a,b,c,\
                                                 homoA,homoB,homoC,\
                                                 compA,compB,compC,\
                                                 aVisible,bVisible,cVisible,\
                                                 left,right,\
                                                 valueAtLeft,valueAtRight,\
                                                 Step,Value) firstprivate(scanLine,mFiller)
#endif  // ENABLE_OPENMP
                for ( size_t i = 0;
                        i < (*mesh)->mFaces->mSize; ++i )
                {

                    itor = (*mesh)->mFaces->mBuffer + i;

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
                    mFiller.nextTriangle( **object, **mesh, *itor );

                    scanLine.reset();
                    compA.depth = homoA[2];
                    compB.depth = homoB[2];
                    compC.depth = homoC[2];

                    mFiller.getCompaionData( 0, *itor, **mesh, **object, homoA, compA.mData );
                    mFiller.getCompaionData( 1, *itor, **mesh, **object, homoB, compB.mData );
                    mFiller.getCompaionData( 2, *itor, **mesh, **object, homoC, compC.mData );

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
                            if ( Value.depth < zBuffer.getValue( a ) ) {
                                zBuffer.setValue( a, Value.depth );
                                mFiller.plot( a, Value.mData, Value.depth, film );
                            }
                        }
                    }
                }
            }
        }
        mFiller.end();
    }
};

#endif /* end of include guard: RASTERISATIONCAMERA_H */
