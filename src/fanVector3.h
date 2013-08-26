#ifndef FANVECTOR3_H

#define FANVECTOR3_H

#include "fanVector.h"
#include <cstring>

namespace fan {

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

};

}

#endif /* end of include guard: FANVECTOR3_H */
