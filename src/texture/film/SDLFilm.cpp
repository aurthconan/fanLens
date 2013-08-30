#include "SDLFilm.h"

using namespace fan;

SDLFilm::SDLFilm( const fan::fanVector<int, 2>& dimensions )
    : fanFilm( dimensions ) {
}

SDLFilm::~SDLFilm() {
}

fanPixel SDLFilm::getPixel( fanVector<int, 2> index ) const {
    (void)index;
    return fanPixel();
}

void SDLFilm::setPixel( const fanVector<int, 2>& index,
                        const fanPixel& pixel ) {
    (void)index; (void)pixel;
}

