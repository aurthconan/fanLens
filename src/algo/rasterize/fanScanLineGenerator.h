#ifndef FANSCANLINEGENERATOR_H

#define FANSCANLINEGENERATOR_H

#include <fanVector.h>

namespace fan {

template<typename CompaionDataType>
class fanScanLineGenerator
{
public:
    fanScanLineGenerator(  const fan::fanVector<int, 2>& dimensions )
        : mLines( new unsigned short[dimensions[1]] )
        , mXLeft( new float[dimensions[1]] )
        , mXRight( new float[dimensions[1]] )
        , mLeft( new CompaionDataType[dimensions[1]] )
        , mRight( new CompaionDataType[dimensions[1]] )
        , mYMin( 0 )
        , mYMax( dimensions[1]-1 )
        , mDimens( dimensions )
    {
        reset();
    }

    ~fanScanLineGenerator() {
        if ( mLines ) {
            delete[]  mLines;
        }
        if ( mXLeft ) {
            delete[] mXLeft;
        }
        if ( mXRight ) {
            delete[] mXRight;
        }
        if ( mLeft ) {
            delete[] mLeft;
        }
        if ( mRight ) {
            delete[] mRight;
        }
    }

    void reset() {
        for ( int i = mYMin; i <= mYMax; ++i ) {
            mLines[i] = 0;
        }
        mYMin = mDimens[1]-1;
        mYMax = 0;
    }

    void AddLine( const fanVector<float, 2>& a,
                  const fanVector<float, 2>& b,
                  const CompaionDataType& dataA,
                  const CompaionDataType& dataB ) {
        int y1 = a[1], y2 = b[1];
        if ( y1 == y2 ) {
            if ( (y1 < 0 ) || (y1 >= mDimens[1] ) ) {
                return;
            }
            ScanLineAdd( a[0], y1, dataA );
            ScanLineAdd( b[0], y2, dataB );
        } else {
            if ( y1 < y2 ) {
                Loop( y1, a[0], y2, b[0], dataA, dataB );
            } else {
                Loop( y2, b[0], y1, a[0], dataB, dataA );
            }
        }
    }

    void Loop( int y1, float x1, int y2, float x2,
            const CompaionDataType& d1,
            const CompaionDataType& d2 ) {
        if ( y1 > y2 || (y1 < 0 && y2 < 0)
                || (y1 >= mDimens[1] && y2 >= mDimens[1]) )
            return;
        CompaionDataType startData = d1;
        CompaionDataType step = (d2 - d1)/(y2-y1);
        float stepX = (x2-x1)/(y2-y1);
        if ( y1 < 0 ) {
            startData += step * -y1;
            x1 += stepX * - y1;
            y1 = 0;
        }
        y2 = y2<mDimens[1]?y2:mDimens[1]-1;
        if ( y1 > y2 ) {
            return;
        }
        for ( ; y1 <= y2; ++y1 ) {
            ScanLineAdd( x1, y1, startData );
            startData += step;
            x1 += stepX;
        }
    }

    void ScanLineAdd( float x, int y, const CompaionDataType& data ) {
        /*
        if ( y < 0 || y >= mDimens[1] ) {
            return;
        }
        */
        if ( mLines[y] == 0 ) {
            mXLeft[y] = x;
            mLeft[y] = data;
            mLines[y]++;
        } else if ( mLines[y] == 1 ) {
            if ( mXLeft[y] <= x ) {
                mXRight[y] = x;
                mRight[y] = data;
            } else {
                mXRight[y] = mXLeft[y];
                mRight[y] = mLeft[y];
                mXLeft[y] = x;
                mLeft[y] = data;
            }
            mLines[y]++;
        } else {
            if ( x < mXLeft[y] ) {
                mXLeft[y] = x;
                mLeft[y] = data;
            } else if ( x > mXRight[y] ) {
                mXRight[y] = x;
                mRight[y] = data;
            }
        }
        mYMin = mYMin<y?mYMin:y;
        mYMax = mYMax>y?mYMax:y;
    }

    unsigned short* mLines;
    float* mXLeft;
    float* mXRight;
    CompaionDataType* mLeft;
    CompaionDataType* mRight;
    int mYMin, mYMax;
    fan::fanVector<int, 2> mDimens;
};

}

#endif /* end of include guard: FANSCANLINEGENERATOR_H */
