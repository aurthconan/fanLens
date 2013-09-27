#ifndef MEMORYTEXTURE_H

#define MEMORYTEXTURE_H

#include <fanTexture.h>
#include <string.h>

template<typename T, typename T_VALUE, size_t DIMENS>
class MemoryTexture
    : public fan::fanTexture<T, T_VALUE, DIMENS>
{
public:
    MemoryTexture( const fan::fanVector<T, DIMENS>& dimensions )
        : fan::fanTexture<T, T_VALUE, DIMENS>( dimensions )
        , mBuffer( NULL )
        , mNum( 1 )
    {
        mMultiply[0] = 1;
        for ( size_t i = 0; i < DIMENS; ++i ) {
            mNum *= dimensions[i];
            if ( (i+1) < DIMENS ) {
                mMultiply[i+1] = mNum;
            }
        }

        mBuffer = new T_VALUE[mNum];
    }

    ~MemoryTexture() {
        if ( mBuffer ) {
            delete[] mBuffer;
        }
    }

    virtual T_VALUE getValue( fan::fanVector<T, DIMENS> index ) const {
        if ( !mBuffer ) {
            return T_VALUE();
        }
        size_t newIndex = get1dIndex( index );
        return mBuffer[newIndex];
    }

    virtual void setValue( const fan::fanVector<T, DIMENS>& index,
                           const T_VALUE& value ) {
        if ( !mBuffer ) {
            return;
        }
        size_t newIndex = get1dIndex( index );
        mBuffer[newIndex] = value;
    }

    void reset( T_VALUE value ) {
        if ( !mBuffer ) {
            return;
        }

        for ( size_t i = 0; i < mNum; ++i ) {
            mBuffer[i] = value;
        }
    }

private:
    inline size_t get1dIndex( fan::fanVector<T, DIMENS> index ) const {
        size_t newIndex = index * mMultiply;
        return newIndex;
    }

    T_VALUE* mBuffer;
    fan::fanVector<T, DIMENS> mMultiply;
    size_t mNum;
};

#endif /* end of include guard: MEMORYTEXTURE_H */
