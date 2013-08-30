#ifndef FANVECTOR3_H

#define FANVECTOR3_H

#include "fanVector.h"
#include <cstring>

namespace fan {

template<typename T>
fanVector<T, 3> cross( const fanVector<T, 3>& l, const fanVector<T, 3>& r );

template<typename T>
class fanVector3
    : public fanVector<T, 3>
{
public:
    T& x;
    T& y;
    T& z;

    fanVector3( T _x = 0, T _y = 0, T _z = 0 )
        : x(fanVector<T,3>::axis[0])
        , y(fanVector<T,3>::axis[1])
        , z(fanVector<T,3>::axis[2])
    {
        x = _x; y = _y; z = _z;
    }

    fanVector3( const fanVector<T, 3>& other )
        : fanVector<T, 3>( other )
        , x(fanVector<T,3>::axis[0])
        , y(fanVector<T,3>::axis[1])
        , z(fanVector<T,3>::axis[2])
    {
    }

    fanVector3( const fanVector3<T>& other )
        : fanVector<T, 3>( other )
        , x(fanVector<T,3>::axis[0])
        , y(fanVector<T,3>::axis[1])
        , z(fanVector<T,3>::axis[2])
    {
    }

    fanVector3<T>& operator=( const fanVector3<T>& other ) {
        x = other.x; y = other.y; z = other.z;
    }
};

template<typename T>
fanVector<T, 3> cross( const fanVector<T, 3>& l, const fanVector<T, 3>& r ) {
    fanVector<T, 3> result;
    result[0] = l[1]*r[2] - l[2]*r[1];
    result[1] = l[2]*r[0] - l[0]*r[2];
    result[2] = l[0]*r[1] - l[1]*r[0];
    return result;
}

}

#endif /* end of include guard: FANVECTOR3_H */
