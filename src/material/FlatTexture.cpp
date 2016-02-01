#include "FlatTexture.h"

using namespace fan;

FlatTexture::FlatTexture( fanTexture<int, fanPixel, 2>* texture )
    : mTexture( texture )
{
}

fanPixel FlatTexture::getPixel( fanVector<float, 2>& dimens,
                                fanPixel incomming )
{
    if ( !mTexture ) {
        return fanPixel();
    }
    fanVector<int, 2> pos = mTexture->getDimens();
    pos[0] *= dimens[0];
    pos[1] *= dimens[1];
    return mTexture->getValue( pos );
}

