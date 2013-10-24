#ifndef FANVECTOR_H

#define FANVECTOR_H

#include <cmath>
#include <cstring>
#include <algorithm>

#include <initializer_list>

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

template<typename T, size_t DIMENS_L, size_t DIMENS_R>
inline bool equal( const fanVector<T, DIMENS_L>& l,
                   const fanVector<T, DIMENS_R>& r );

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

    fanVector( std::initializer_list<T> list ) {
        size_t length = list.size();
        length = length>DIMENS?DIMENS:length;
        auto value = list.begin();
        for ( size_t i = 0; i < length; ++i, ++value ) {
            axis[i] = *value;
        }
        for ( size_t i = length; i < DIMENS; ++i ) {
            axis[i] = 0;
        }
    }

    template<typename OTHER_T>
    fanVector( const fanVector<OTHER_T, DIMENS>& other ) {
        for ( size_t i = 0; i < DIMENS; ++i ) {
            axis[i] = static_cast<T>(other.axis[i]);
        }
    }

    template<typename OTHER_T, size_t OTHER_DIMENS>
    fanVector( const fanVector<OTHER_T, OTHER_DIMENS>& other ) {
        size_t end = std::min( DIMENS, OTHER_DIMENS );
        for ( size_t i = 0; i < end; ++i ) {
            axis[i] = other.axis[i];
        }
        for ( size_t i = end; i < DIMENS; ++i ) {
            axis[i] = 0;
        }
    }

    fanVector( const fanVector<T, DIMENS>& other ) {
        memcpy( axis, other.axis, sizeof(axis) );
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

    template<typename OTHER_T, size_t OTHER_DIMENS>
    inline fanVector<T, DIMENS> operator+( const fanVector<OTHER_T, OTHER_DIMENS> other ) const {
        fanVector<T, DIMENS> result;
        size_t end = std::min( DIMENS, OTHER_DIMENS );
        for ( size_t i = 0; i < end; ++i ) {
            result.axis[i] = axis[i] + other.axis[i];
        }
        for ( size_t i = end; i < DIMENS; ++i ) {
            result.axis[i] = axis[i];
        }
        return result;
    }

    template<typename OTHER_T, size_t OTHER_DIMENS>
    inline fanVector<T, DIMENS>& operator+=( const fanVector<T, DIMENS> other ) {
        size_t end = std::min( DIMENS, OTHER_DIMENS );
        for ( size_t i = 0; i < end; ++i ) {
            axis[i] += other.axis[i];
        }
        return *this;
    }

    template<typename OTHER_T, size_t OTHER_DIMENS>
    inline fanVector<T, DIMENS> operator-( const fanVector<OTHER_T, OTHER_DIMENS> other ) const {
        fanVector<T, DIMENS> result;
        size_t end = std::min( DIMENS, OTHER_DIMENS );
        for ( size_t i = 0; i < end; ++i ) {
            result.axis[i] = axis[i] - other.axis[i];
        }
        for ( size_t i = end; i < DIMENS; ++i ) {
            result.axis[i] = axis[i];
        }
        return result;
    }

    template<typename OTHER_T, size_t OTHER_DIMENS>
    inline fanVector<T, DIMENS>& operator-=( const fanVector<T, DIMENS> other ) {
        size_t end = std::min( DIMENS, OTHER_DIMENS );
        for ( size_t i = 0; i < end; ++i ) {
            axis[i] -= other.axis[i];
        }
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

    inline const T& operator[]( size_t i ) const {
        return axis[i];
    }

    template<size_t DIMENS_O>
    inline bool operator==( const fanVector<T, DIMENS_O>& other ) const {
        return equal( *this, other );
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
    T result(0);
    for( size_t i = 0; i < DIMENS; ++i ) {
        result += left.axis[i] * right.axis[i];
    }
    return result;
}

template<typename T>
inline T dot( const fanVector<T, 2>& left,
              const fanVector<T, 2>& right ) {
    T result(0);
    result = left.axis[0]*right.axis[0]
            +left.axis[1]*right.axis[1];
    return result;
}

template<typename T>
inline T dot( const fanVector<T, 3>& left,
              const fanVector<T, 3>& right ) {
    T result(0);
    result = left.axis[0]*right.axis[0]
            +left.axis[1]*right.axis[1]
            +left.axis[2]*right.axis[2];
    return result;
}

template<typename T>
inline T dot( const fanVector<T, 4>& left,
              const fanVector<T, 4>& right ) {
    T result(0);
    result = left.axis[0]*right.axis[0]
            +left.axis[1]*right.axis[1]
            +left.axis[2]*right.axis[2]
            +left.axis[3]*right.axis[3];
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

template<typename T, size_t DIMENS_L, size_t DIMENS_R>
inline bool equal( const fanVector<T, DIMENS_L>& l,
                   const fanVector<T, DIMENS_R>& r ) {
    if ( DIMENS_L != DIMENS_R ) {
        return false;
    }
    for ( size_t i = 0; i < DIMENS_R; ++i ) {
        if ( l[i] != r[i] )
            return false;
    }
    return true;
}

}

#endif /* end of include guard: FANVECTOR_H */
