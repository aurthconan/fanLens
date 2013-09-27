#ifndef FANTEXTURE_H

#define FANTEXTURE_H

#include "fanVector.h"

namespace fan {

template<typename T, typename T_VALUE, size_t DIMENS>
class fanTexture
{
public:
    fanTexture( const fanVector<T, DIMENS>& dimensions )
        : mDimensions( dimensions ) {
    }

    virtual ~fanTexture() {
    }

    fanVector<T, DIMENS> getDimens() const {
        return mDimensions;
    }

    virtual T_VALUE getValue( fanVector<T, DIMENS> index ) const {
        (void)index;
        return T_VALUE();
    }

    virtual void setValue( const fanVector<T, DIMENS>& index,
                           const T_VALUE& value ) {
        (void)index; (void)value;
    }

protected:
    fanVector<T, DIMENS> mDimensions;
};

}

#endif /* end of include guard: FANTEXTURE_H */
