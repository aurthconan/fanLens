#ifndef SCANLINESTOREFILM_H

#define SCANLINESTOREFILM_H

#include <fanTexture.h>

#include <ext/hash_map>
#include <utility>

// a class use to store and generate scanline
template<typename T_VALUE>
class ScanLineStoreTexture
    : public fan::fanTexture<int, T_VALUE, 2>
{
public:
    typedef std::pair<int, T_VALUE> EndPointDataType;
    ScanLineStoreTexture( const fan::fanVector<int, 2>& dimensions )
        : fan::fanTexture<int, T_VALUE, 2>( dimensions )
        , mYMin( dimensions[1] )
        , mYMax( 0 )
    {
        reset();
    }

    void reset() {
        fan::fanVector<int, 2> windowDimens =
            fan::fanTexture<int, T_VALUE, 2>::getDimens();

        mYMin = windowDimens[1];
        mYMax = 0;
        mYBucket.clear();
    }

    void setValue( const fan::fanVector<int, 2>& index,
                   const T_VALUE& value ) {
        fan::fanVector<int, 2> windowDimens =
            fan::fanTexture<int, T_VALUE, 2>::getDimens();

        if ( index[1] < 0 || index[1] >= windowDimens[1] ) {
            return;
        }

        if ( index[1] < mYMin ) {
            mYMin = index[1];
        } else if ( index[1] > mYMax ) {
            mYMax = index[1];
        }

        std::pair<EndPointDataType, EndPointDataType> data;
        if ( mYBucket.find( index[1] ) != mYBucket.end() ) {
            data = mYBucket[index[1]];
        } else {
            data.first.first = index[0];
            data.first.second = value;
            data.second.first = index[0];
            data.second.second = value;
        }
        if ( index[0] < data.first.first ) {
            data.first.first = index[0];
            data.first.second = value;
        } else if ( index[0] > data.second.first ) {
            data.second.first = index[0];
            data.second.second = value;
        }
        mYBucket[index[1]] = data;
    }

    int mYMin;
    int mYMax;

    __gnu_cxx::hash_map<int, std::pair<EndPointDataType,
                                 EndPointDataType> > mYBucket;
};

#endif /* end of include guard: SCANLINESTOREFILM_H */
