#ifndef FANTEXTURE_H

#define FANTEXTURE_H

#include "fanVector.h"
#include "fanPixel.h"

namespace fan {

template<typename T, size_t DIMENS>
class fanTexture
{
public:
    fanTexture( const fanVector<T, DIMENS>& dimensions );
    virtual ~fanTexture();

    fanVector<T, DIMENS> getDimens();

    fanPixel getPixel( fanVector<T, DIMENS> index ) const;
    void setPixel( const fanVector<T, DIMENS>& index, const fanPixel& pixel );

protected:
    fanVector<T, DIMENS> mDimensions;
};

template<typename T, size_t DIMENS>
fanTexture::fanTexture( const fanVector<T, DIMENS>& dimensions )
    : mDimensions( dimensions ) {
}

template<typename T, size_t DIMENS>
fanVector<T, DIMENS> fanTexture::getDimens() {
    return mDimensions;
}

fanPixel fanTexture::getPixel( fanVector<T, DIMENS> index ) const {
    return fanPixel();
}

void fanTexture::setPixel( const fanVector<T, DIMENS>& index, const fanPixel& pixel ) {
}


}

#endif /* end of include guard: FANTEXTURE_H */
