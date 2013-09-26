#include "ScanLineStoreFilm.h"

#include <stdlib.h>

using namespace fan;

ScanLineStoreFilm::ScanLineStoreFilm( const fanVector<int, 2>& dimensions )
    : fanFilm( dimensions )
    , mYMin( dimensions[1] )
    , mYMax( 0 )
    , mpYBucket( (fanVector<int, 2>*)
                        malloc(sizeof(fanVector<int, 2>)*dimensions[1]) )
{
    reset();
}

void ScanLineStoreFilm::reset() {
    fanVector<int, 2> windowDimens = getDimens();

    fanVector<int, 2> x;
    x[0] = windowDimens[0];
    x[1] = 0;

    for ( size_t i = 0, max = windowDimens[1]; i < max; ++i ) {
        mpYBucket[i] = x;
    }
    mYMin = windowDimens[1];
    mYMax = 0;
}

ScanLineStoreFilm::~ScanLineStoreFilm() {
    free(mpYBucket);
}

void ScanLineStoreFilm::setPixel( const fanVector<int, 2>& index,
                                  const fanPixel& pixel ) {
    (void) pixel;
    fanVector<int, 2> _index = index;
    fanVector<int, 2> windowDimens = getDimens();

    if ( _index[1] < 0 || _index[1] >= windowDimens[1] ) {
        return;
    }

    if ( windowDimens[0] < _index[0] ) {
        _index[0] = windowDimens[0] - 1;
    } else if ( 0 > _index[0] ) {
        _index[0] = 0;
    }

    if ( _index[1] < mYMin ) {
        mYMin = _index[1];
    } else if ( _index[1] > mYMax ) {
        mYMax = _index[1];
    }

    fanVector<int, 2>* entry = mpYBucket + _index[1];
    if ( _index[0] < (*entry)[0] ) {
        (*entry)[0] = _index[0];
    }
    if ( _index[0] > (*entry)[1] ) {
        (*entry)[1] = _index[0];
    }
}

