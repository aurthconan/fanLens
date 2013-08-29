#include "SDLFilm.h"

using namespace fan;

SDLFilm::SDLFilm( const fan::fanVector<int, 2>& dimensions )
    : fanFilm( dimensions ) {
}

SDLFilm::~SDLFilm() {
}

fanPixel SDLFilm::getPixel( fanVector<int, 2> index ) const {
    return fanPixel();
}

void SDLFilm::setPixel( const fanVector<int, 2>& index,
                        const fanPixel& pixel ) {
}

