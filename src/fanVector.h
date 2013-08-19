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
    fanVector();
    ~fanVector();

    fanVector( T value )
        : fanTriple( value, value, value ) {}

    fanVector( T _x = 0, T _y = 0, T _z = 0 )
        : fanTriple( _x, _y, _z ) {}

    inline fanVector<T> operator+( const fanVector<T> other ) const {
        return fanVector( x += other.x, y += other.y, z += other.z );
    }

    inline fanVector<T>& operator+=( const fanVector<T> other ) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    inline fanVector<T> operator*( const T& scale ) const {
        return fanVector( x * scale, y * scale, z * scale );
    }

    inline fanVector<T>& operator*=( const T& scale ) {
        x *= scale; y *= scale; z *= scale;
        return *this;
    }

    inline T operator*( const fanVector<T>& other ) const {
        return x * other.x + y * other.y + z * other.z;
    }

    inline T lengthSquare() {
        return x * x + y * y + z * z;
    }

    inline T length() {
        std::sqrt( lengthSquare() );
    }

    inline void normalize() {
        T w = length();
        x /= w; y /= w; z /= w;
    }
private:
};

}

#endif /* end of include guard: FANVECTOR_H */
