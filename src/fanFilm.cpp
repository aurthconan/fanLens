#include "fanFilm.h"

namespace fan {

fanFilm::fanFilm( const fanVector<int, 2>& dimensions )
    : fanTexture<int, fanPixel, 2>( dimensions ) {
}

fanFilm::~fanFilm() {
}

void fanFilm::develope() {
}

}

