#include "fanTriangle.h"

namespace fan {

fanTriangle::fanTriangle( fanVector3<float>* _a,
                          fanVector3<float>* _b,
                          fanVector3<float>* _c,
                          fanVector3<float> center,
                          fanVector3<float> normal )
    : a(_a)
    , b(_b)
    , c(_c)
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

