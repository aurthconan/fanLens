#include "FreeImageFilm.h"

#include <FreeImage.h>

using namespace fan;

bool FreeImageFilm::sbFreeImageInitialized = false;

FreeImageFilm::FreeImageFilm( const fan::fanVector<int, 2>& dimensions,
                              const std::string file )
    : fanFilm( dimensions )
    , mFileName( file )
    , mpBuffer( NULL ) {
    if ( !sbFreeImageInitialized ) {
        FreeImage_Initialise();
    }
    mpBuffer = new fanPixel[dimensions[0] * dimensions[1]];
}

FreeImageFilm::~FreeImageFilm() {
    fanVector<int, 2> dimensions = getDimens();
    FIBITMAP *img = FreeImage_ConvertFromRawBits((BYTE*)mpBuffer, dimensions[0],
                                dimensions[1], dimensions[0] * 3, 32,
                                0xFF0000, 0x00FF00, 0x0000FF, false);

    FreeImage_Save(FIF_PNG, img, mFileName.c_str(), 0);

    if ( sbFreeImageInitialized ) {
        FreeImage_DeInitialise();
    }

    delete[] mpBuffer;
}

fanPixel FreeImageFilm::getPixel( fan::fanVector<int, 2> index ) const {
    return mpBuffer[index[1]*getDimens()[0] + index[0]];
}

void FreeImageFilm::setPixel( const fan::fanVector<int, 2>& index,
                              const fan::fanPixel& pixel ) {
    mpBuffer[index[1]*getDimens()[0] + index[0]] = pixel;
}

