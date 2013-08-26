#ifndef FANVECTOR_H

#define FANVECTOR_H

#include <cmath>
#include <cstring>

namespace fan {

template<typename T, size_t DIMENS>
class fanVector
{
public:
    static const size_t dimensNum = DIMENS;
    union {
        T axis[DIMENS];
    };

    virtual ~fanVector() {}

    fanVector()
    {
        memset( axis, 0, sizeof(axis) );
    }

    //template<typename RET_T>
    inline fanVector<T, DIMENS> operator+( const fanVector<T, DIMENS> other ) const {
        fanVector<T, DIMENS> result;
        for ( size_t i = 0; i < DIMENS; ++i ) {
            result.axis[i] = axis[i] + other.axis[i];
        }
        return result;
    }

    inline fanVector<T, DIMENS>& operator+=( const fanVector<T, DIMENS> other ) {
        for ( size_t i = 0; i < DIMENS; ++i ) {
            axis[i] += other.axis[i];
        }
        return *this;
    }

    inline fanVector<T, DIMENS> operator*( const T& scale ) const {
        fanVector<T, DIMENS> result;
        for ( size_t i = 0; i < DIMENS; ++i ) {
            result.axis[i] = axis[i] * scale;
        }
        return result;
    }

    inline fanVector<T, DIMENS>& operator*=( const T& scale ) {
        for ( size_t i = 0; i < DIMENS; ++i ) {
            axis[i] *= scale;
        }
        return *this;
    }

    inline T operator*( const fanVector<T, DIMENS>& other ) const {
        T result(0);
        for ( size_t i = 0; i < DIMENS; ++i ) {
            result += axis[i] * other.axis[i];
        }
        return result;
    }

    inline T lengthSquare() {
        T result(0);
        for ( size_t i = 0; i < DIMENS; ++i ) {
            result += axis[i] * axis[i];
        }
        return result;
    }

    inline T length() {
        return std::sqrt( lengthSquare() );
    }

    inline void normalize() {
        T w = length();
        for ( size_t i = 0; i < DIMENS; ++i ) {
            axis[i] /= w;
        }
    }
private:
};

}

#endif /* end of include guard: FANVECTOR_H */
