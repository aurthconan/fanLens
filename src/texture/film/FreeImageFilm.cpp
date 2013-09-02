#include "FreeImageFilm.h"

#include <FreeImage.h>

using namespace fan;

bool FreeImageFilm::sbFreeImageInitialized = false;

FreeImageFilm::FreeImageFilm( const fan::fanVector<int, 2>& dimensions,
                              const std::string file )
    : fanFilm( dimensions )
    , mFileName( file )
    , mpBitmap( NULL ) {
    if ( !sbFreeImageInitialized ) {
        FreeImage_Initialise();
    }

    mpBitmap = FreeImage_Allocate( dimensions[0], dimensions[1], 32 );
}

FreeImageFilm::~FreeImageFilm() {
    if ( mpBitmap ) {
        FreeImage_Unload(mpBitmap);
        mpBitmap = NULL;
    }

    if ( sbFreeImageInitialized ) {
        FreeImage_DeInitialise();
    }

}

void FreeImageFilm::develope() {
    if ( mpBitmap ) {
        FreeImage_Save(FIF_PNG, mpBitmap, mFileName.c_str(), 0);
    }
}

fanPixel FreeImageFilm::getPixel( fan::fanVector<int, 2> index ) const {
    if ( !mpBitmap ) {
        return fanPixel();
    }

    RGBQUAD color;
    if ( FreeImage_GetPixelColor( mpBitmap, index[0], index[1], &color ) ) {
        return fanPixel( 0,
                         color.rgbRed,
                         color.rgbGreen,
                         color.rgbBlue );
    } else {
        return fanPixel();
    }
}

void FreeImageFilm::setPixel( const fan::fanVector<int, 2>& index,
                              const fan::fanPixel& pixel ) {
    if ( !mpBitmap ) {
        return;
    }

    RGBQUAD color;
    color.rgbRed = pixel.r;
    color.rgbGreen = pixel.g;
    color.rgbBlue = pixel.b;
    color.rgbReserved = pixel.a;
    FreeImage_SetPixelColor( mpBitmap, index[0], index[1], &color );
}

