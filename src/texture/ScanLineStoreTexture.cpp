#include "ScanLineStoreTexture.h"

#include <stdlib.h>

using namespace fan;

ScanLineStoreTexture::ScanLineStoreTexture( const fanVector<int, 2>& dimensions )
    : fanTexture<int, float, 2>( dimensions )
    , mYMin( dimensions[1] )
    , mYMax( 0 )
    , mpYBucket( (fanVector<int, 2>*)
                        malloc(sizeof(fanVector<int, 2>)*dimensions[1]) )
    , mpYValues( (fanVector<float, 2>*)
                        malloc(sizeof(fanVector<float, 2>)*dimensions[1]) )
{
    reset();
}

void ScanLineStoreTexture::reset() {
    fanVector<int, 2> windowDimens = getDimens();

    fanVector<int, 2> x;
    x[0] = windowDimens[0];
    x[1] = 0;

    fanVector<float, 2> value;
    value[0] = .0f; value[1] = .0f;

    for ( size_t i = 0, max = windowDimens[1]; i < max; ++i ) {
        mpYBucket[i] = x;
        mpYValues[i] = value;
    }
    mYMin = windowDimens[1];
    mYMax = 0;
}

ScanLineStoreTexture::~ScanLineStoreTexture() {
    free(mpYBucket);
    free(mpYValues);
}

void ScanLineStoreTexture::setValue( const fanVector<int, 2>& index,
                                  const float& value ) {
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
    fanVector<float, 2>* valueEntry = mpYValues + _index[1];
    if ( _index[0] < (*entry)[0] ) {
        (*entry)[0] = _index[0];
        (*valueEntry)[0] = value;
    }
    if ( _index[0] > (*entry)[1] ) {
        (*entry)[1] = _index[0];
        (*valueEntry)[1] = value;
    }
}

