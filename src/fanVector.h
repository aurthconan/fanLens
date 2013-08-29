#ifndef FANVECTOR_H

#define FANVECTOR_H

#include <cmath>
#include <cstring>

namespace fan {

template<typename T, size_t DIMENS>
class fanVector;

template<typename T, size_t DIMENS>
inline fanVector<T, DIMENS> add( const fanVector<T, DIMENS>& left,
                                 const fanVector<T, DIMENS>& right );

template<typename T, size_t DIMENS>
inline fanVector<T, DIMENS> subtract( const fanVector<T, DIMENS>& left,
                                      const fanVector<T, DIMENS>& right );

template<typename T, size_t DIMENS, typename SCALE_T>
inline fanVector<T, DIMENS> multiply( const fanVector<T, DIMENS>& operend,
                                      const SCALE_T& scale );

template<typename T, size_t DIMENS, typename SCALE_T>
inline fanVector<T, DIMENS> divide( const fanVector<T, DIMENS>& operend,
                                    const SCALE_T& scale );

template<typename T, size_t DIMENS>
inline T dot( const fanVector<T, DIMENS>& left,
              const fanVector<T, DIMENS>& right );

template<typename T, size_t DIMENS>
inline T distanceSquare( const fanVector<T, DIMENS>& left,
                         const fanVector<T, DIMENS>& right );

template<typename T, size_t DIMENS>
inline T distance( const fanVector<T, DIMENS>& left,
                   const fanVector<T, DIMENS>& right );

template<typename T, size_t DIMENS>
inline T lengthSquare( const fanVector<T, DIMENS>& operend );

template<typename T, size_t DIMENS>
inline T length( const fanVector<T, DIMENS>& operend );

template<typename T, size_t DIMENS>
inline fanVector<T, DIMENS> normalize( const fanVector<T, DIMENS>& operend );

template<typename T, size_t DIMENS>
class fanVector
{
public:
    static const size_t dimensNum = DIMENS;
    union {
        T axis[DIMENS];
    };

    fanVector()
    {
        memset( axis, 0, sizeof(axis) );
    }

    inline fanVector<T, DIMENS> operator+( const fanVector<T, DIMENS> other ) const {
        return add( *this, other );
    }

    inline fanVector<T, DIMENS>& operator+=( const fanVector<T, DIMENS> other ) {
        *this = add( *this, other );
        return *this;
    }

    inline fanVector<T, DIMENS> operator-( const fanVector<T, DIMENS> other ) const {
        return subtract( *this, other );
    }

    inline fanVector<T, DIMENS>& operator-=( const fanVector<T, DIMENS> other ) {
        *this = subtract( *this, other );
        return *this;
    }

    inline fanVector<T, DIMENS> operator/( const T& scale ) const {
        return divide( *this, scale );
    }

    inline fanVector<T, DIMENS>& operator/=( const T& scale ) {
        *this = divide( *this, scale );
        return *this;
    }

    inline fanVector<T, DIMENS> operator*( const T& scale ) const {
        return multiply( *this, scale );
    }

    inline fanVector<T, DIMENS>& operator*=( const T& scale ) {
        *this = multiply( *this, scale );
        return *this;
    }


    inline T operator*( const fanVector<T, DIMENS>& other ) const {
        return dot( *this, other );
    }

    inline T lengthSquare() {
        return fan::lengthSquare( *this );
    }

    inline T length() {
        return fan::length( *this );
    }

    inline void normalize() {
        *this = fan::normalize( *this );
    }

    inline T& operator[]( size_t i ) {
        return axis[i];
    }

    inline T operator[]( size_t i ) const {
        return axis[i];
    }
};

template<typename T, size_t DIMENS>
inline fanVector<T, DIMENS> add( const fanVector<T, DIMENS>& left,
                                 const fanVector<T, DIMENS>& right ) {
    fanVector<T, DIMENS> result;
    for ( size_t i = 0; i < DIMENS; ++i ) {
        result.axis[i] = left.axis[i] + right.axis[i];
    }
    return result;
}

template<typename T, size_t DIMENS>
inline fanVector<T, DIMENS> subtract( const fanVector<T, DIMENS>& left,
                                      const fanVector<T, DIMENS>& right ) {
    fanVector<T, DIMENS> result;
    for ( size_t i = 0; i < DIMENS; ++i ) {
        result.axis[i] = left.axis[i] - right.axis[i];
    }
    return result;
}


template<typename T, size_t DIMENS, typename SCALE_T>
inline fanVector<T, DIMENS> multiply( const fanVector<T, DIMENS>& operend,
                                      const SCALE_T& scale ) {
    fanVector<T, DIMENS> result;
    for ( size_t i = 0; i < DIMENS; ++i ) {
        result.axis[i] = operend.axis[i] * scale;
    }
    return result;
}

template<typename T, size_t DIMENS, typename SCALE_T>
inline fanVector<T, DIMENS> divide( const fanVector<T, DIMENS>& operend,
                                    const SCALE_T& scale ) {
    fanVector<T, DIMENS> result;
    for ( size_t i = 0; i < DIMENS; ++i ) {
        result.axis[i] = operend.axis[i] / scale;
    }
    return result;
}

template<typename T, size_t DIMENS>
inline T dot( const fanVector<T, DIMENS>& left,
              const fanVector<T, DIMENS>& right ) {
    T result;
    for( size_t i = 0; i < DIMENS; ++i ) {
        result += left.axis[i] * right.axis[i];
    }
    return result;
}

template<typename T, size_t DIMENS>
inline T distanceSquare( const fanVector<T, DIMENS>& left,
                         const fanVector<T, DIMENS>& right ) {
    T result(0);
    for( size_t i = 0; i < DIMENS; ++i ) {
        T distance = left.axis[i] - right.axis[i];
        result += distance * distance;
    }
    return result;
}

template<typename T, size_t DIMENS>
inline T distance( const fanVector<T, DIMENS>& left,
                   const fanVector<T, DIMENS>& right ) {
    return std::sqrt( distanceSquare( left, right ) );
}

template<typename T, size_t DIMENS>
inline T lengthSquare( const fanVector<T, DIMENS>& operend ) {
    fanVector<T, DIMENS> origin;
    return distanceSquare( operend, origin );
}

template<typename T, size_t DIMENS>
inline T length( const fanVector<T, DIMENS>& operend ) {
    return std::sqrt( lengthSquare( operend ) );
}

template<typename T, size_t DIMENS>
inline fanVector<T, DIMENS> normalize( const fanVector<T, DIMENS>& operend ) {
    return divide( operend, length( operend ) );
}

}

#endif /* end of include guard: FANVECTOR_H */
