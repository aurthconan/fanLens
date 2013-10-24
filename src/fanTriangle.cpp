#include "fanTriangle.h"

namespace fan {

fanTriangle::fanTriangle()
    : points{ 0, 0, 0 }
    , pointsIndex{ 0, 0, 0 }
    , mCenter( )
    , mNormal( ) {
}

fanTriangle::fanTriangle( fanVector3<float>* _a,
                          fanVector3<float>* _b,
                          fanVector3<float>* _c,
                          size_t _aIndex,
                          size_t _bIndex,
                          size_t _cIndex,
                          fanVector3<float> center,
                          fanVector3<float> normal )
    : points{ _a, _b, _c }
    , pointsIndex{ _aIndex, _bIndex, _cIndex }
    , mCenter( center )
    , mNormal( normal ) {

    if ( mCenter == fanVector3<float>() ) {
        mCenter = ( *points[0] + *points[1] + *points[2] ) / 3;
    }
    if ( mNormal == fanVector3<float>() ) {
        mNormal = cross( *points[1] - *points[0],
                         *points[2] - *points[1] );
        mNormal.normalize();
    }
}

}

