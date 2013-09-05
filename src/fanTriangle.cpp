#include "fanTriangle.h"

namespace fan {

fanTriangle::fanTriangle( fanVector3<float>& _a,
                          fanVector3<float>& _b,
                          fanVector3<float>& _c )
    : a(_a)
    , b(_b)
    , c(_c) {
}

fanTriangle::fanTriangle( const  fanTriangle& other )
    : a(other.a)
    , b(other.b)
    , c(other.c) {
}

}

