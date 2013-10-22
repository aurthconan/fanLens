#include "fanTriangle.h"

namespace fan {

fanTriangle::fanTriangle()
    : a(0)
    , b(0)
    , c(0)
    , aIndex( 0 )
    , bIndex( 0 )
    , cIndex( 0 )
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
    : a(_a)
    , b(_b)
    , c(_c)
    , aIndex( _aIndex )
    , bIndex( _bIndex )
    , cIndex( _cIndex )
    , mCenter( center )
    , mNormal( normal ) {

    if ( mCenter == fanVector3<float>() ) {
        mCenter = ( *a + *b + *c ) / 3;
    }
    if ( mNormal == fanVector3<float>() ) {
        mNormal = cross( *b - *a, *c - *b );
        mNormal.normalize();
    }
}

}

