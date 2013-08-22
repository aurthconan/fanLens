#ifndef FANVECTOR_H

#define FANVECTOR_H

#include <cmath>
#include "fanTriple.h"

namespace fan {

template<typename T>
class fanVector
    : public fanTriple<T>
{
public:
    virtual ~fanVector() {}

    fanVector( T _x = 0, T _y = 0, T _z = 0 )
        : fanTriple<T>( _x, _y, _z ) {}

    inline fanVector<T> operator+( const fanVector<T> other ) const {
        return fanVector( fanTriple<T>::x + other.x,
                          fanTriple<T>::y + other.y,
                          fanTriple<T>::z + other.z );
    }

    inline fanVector<T>& operator+=( const fanVector<T> other ) {
        fanTriple<T>::x += other.x;
        fanTriple<T>::y += other.y;
        fanTriple<T>::z += other.z;
        return *this;
    }

    inline fanVector<T> operator*( const T& scale ) const {
        return fanVector( fanTriple<T>::x * scale,
                          fanTriple<T>::y * scale,
                          fanTriple<T>::z * scale );
    }

    inline fanVector<T>& operator*=( const T& scale ) {
        fanTriple<T>::x *= scale;
        fanTriple<T>::y *= scale;
        fanTriple<T>::z *= scale;
        return *this;
    }

    inline T operator*( const fanVector<T>& other ) const {
        return  fanTriple<T>::x * other.x
              + fanTriple<T>::y * other.y
              + fanTriple<T>::z * other.z;
    }

    inline T lengthSquare() {
        return  fanTriple<T>::x * fanTriple<T>::x
              + fanTriple<T>::y * fanTriple<T>::y
              + fanTriple<T>::z * fanTriple<T>::z;
    }

    inline T length() {
        return std::sqrt( lengthSquare() );
    }

    inline void normalize() {
        T w = length();
        fanTriple<T>::x /= w; fanTriple<T>::y /= w; fanTriple<T>::z /= w;
    }
private:
};

}

#endif /* end of include guard: FANVECTOR_H */
