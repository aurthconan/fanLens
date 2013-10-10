#ifndef SCANLINESTOREFILM_H

#define SCANLINESTOREFILM_H

#include <fanTexture.h>

#include <utility>

template<typename T>
class ScanLineRange {
public:
    int xMin;
    int xMax;
    T valueAtMin;
    T valueAtMax;
};

// a class use to store and generate scanline
template<typename T_VALUE>
class ScanLineStoreTexture
    : public fan::fanTexture<int, T_VALUE, 2>
{
public:
    typedef std::pair<int, T_VALUE> EndPointDataType;
    ScanLineStoreTexture( const fan::fanVector<int, 2>& dimensions )
        : fan::fanTexture<int, T_VALUE, 2>( dimensions )
        , mYMin( 0 )
        , mYMax( dimensions[1] - 1 )
        , mYBucket( new ScanLineRange<T_VALUE>[dimensions[1]] )
    {
        reset();
    }

    ~ScanLineStoreTexture() {
        if ( mYBucket ) {
            delete[] mYBucket;
        }
    }

    void reset() {
        ScanLineRange<T_VALUE>* oneLine;
        for ( int i = mYMin, max = mYMax;
                i <= max; ++i ) {
            oneLine = mYBucket + i;
            oneLine->xMin = fan::fanTexture<int, T_VALUE, 2>::mDimensions[0];
            oneLine->xMax = 0;
        }

        mYMin = fan::fanTexture<int, T_VALUE, 2>::mDimensions[1] - 1;
        mYMax = 0;
    }

    void setValue( const fan::fanVector<int, 2>& index,
                   const T_VALUE& value ) {

        if ( index[1] < 0 ||
                index[1] >= fan::fanTexture<int, T_VALUE, 2>::mDimensions[1] ) {
            return;
        }

        if ( index[1] < mYMin ) {
            mYMin = index[1];
        }
        if ( index[1] > mYMax ) {
            mYMax = index[1];
        }

        ScanLineRange<T_VALUE>* oneLine = mYBucket + index[1];

        if ( index[0] < oneLine->xMin ) {
            oneLine->xMin = index[0];
            oneLine->valueAtMin = value;
        }
        if ( index[0] > oneLine->xMax ) {
            oneLine->xMax = index[0];
            oneLine->valueAtMax = value;
        }
    }

    int mYMin;
    int mYMax;

    ScanLineRange<T_VALUE>* mYBucket;
};

#endif /* end of include guard: SCANLINESTOREFILM_H */
