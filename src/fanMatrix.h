#ifndef FANMATRIX_H

#define FANMATRIX_H

#include "fanVector.h"

namespace fan {

template<typename T, size_t R, size_t C>
class fanMatrix;

template<typename T, size_t R, size_t C>
inline fanMatrix<T, R, C> add( const fanMatrix<T, R, C>& left,
                               const fanMatrix<T, R, C>& right );

template<typename T, size_t R, size_t C>
inline fanMatrix<T, R, C> multiply( const fanMatrix<T, R, C>& operend,
                                    T scale );

template<typename T, size_t D, size_t L_R, size_t R_C>
inline fanMatrix<T, L_R, R_C> multiply( const fanMatrix<T, L_R, D>& left,
                                        const fanMatrix<T, D, R_C>& right );

template<typename T, size_t D, size_t R>
inline fanVector<T, R> multiply( const fanMatrix<T, R, D>& left,
                                 const fanVector<T, D>& right );

template<typename T, size_t R, size_t C>
inline fanMatrix<T, C, R> transpose( const fanMatrix<T, R, C>& operend );

template<typename T, size_t R, size_t C>
class fanMatrix
{
public:
    static const size_t rowsNum = R;
    static const size_t colsNum = C;
    union {
        fanVector<T, C> rows[R];
    };

    fanMatrix()
        : rows()
    {
    };

    inline fanVector<T, C>& operator[]( size_t i ) {
        return rows[i];
    }

    inline fanVector<T, C> operator[]( size_t i ) const {
        return rows[i];
    }

    inline fanMatrix<T, R, C> operator+( const fanMatrix<T, R, C> other ) const {
        return add( *this, other );
    }

    inline fanMatrix<T, R, C>& operator+=( const fanMatrix<T, R, C> other ) {
        *this = add( *this, other );
        return *this;
    }

    inline fanMatrix<T, R, C> operator*( const T& scale ) const {
        return multiply( *this, scale );
    }

    inline fanMatrix<T, R, C>& operator*=( const T& scale ) {
        *this = multiply( *this, scale );
        return *this;
    }

    inline fanVector<T, R> operator*( const fanVector<T, C>& vector ) const {
        return multiply( *this, vector );
    }

    inline fanVector<T, R>& operator*=( const fanVector<T, C>& vector ) {
        *this = multiply( *this, vector );
        return *this;
    }

    template<size_t R_C>
    inline fanMatrix<T, R, R_C> operator*( const fanMatrix<T, C, R_C>& right ) const {
        return multiply( *this, right );
    }
};

template<typename T, size_t R, size_t C>
inline fanMatrix<T, R, C> add( const fanMatrix<T, R, C>& left,
                               const fanMatrix<T, R, C>& right ) {
    fanMatrix<T, R, C> result;
    for ( size_t i = 0; i < R; ++i ) {
        result[i] = left[i] + right[i];
    }
    return result;
}

template<typename T, size_t R, size_t C>
inline fanMatrix<T, R, C> multiply( const fanMatrix<T, R, C>& operend,
                                    T scale ) {
    fanMatrix<T, R, C> result;
    for ( size_t i = 0; i < R; ++i ) {
        result[i] = operend[i] * scale;
    }
    return result;
}

template<typename T, size_t D, size_t L_R, size_t R_C>
inline fanMatrix<T, L_R, R_C> multiply( const fanMatrix<T, L_R, D>& left,
                                        const fanMatrix<T, D, R_C>& right ) {
    fanMatrix<T, L_R, R_C> result;
    for ( size_t i = 0; i < L_R; ++i ) {
        for ( size_t j = 0; j < R_C; ++j ) {
            T value(0);
            for ( size_t l = 0; l < D; ++l ) {
                value += left[i][l] * right[l][j];
            }
            result[i][j] = value;
        }
    }
    return result;
}

template<typename T, size_t D, size_t R>
inline fanVector<T, R> multiply( const fanMatrix<T, R, D>& left,
                                 const fanVector<T, D>& right ) {
    fanVector<T, R> result;
    for ( size_t i = 0; i < R; ++i ) {
        result[i] = left[i] * right;
    }
    return result;
}

template<typename T, size_t R, size_t C>
inline fanMatrix<T, C, R> transpose( const fanMatrix<T, R, C>& operend ) {
    fanMatrix<T, C, R> result;
    for ( size_t i = 0; i < R; ++i ) {
        for ( size_t j = 0; j < C; ++j ) {
            result[j][i] = operend[i][j];
        }
    }
    return result;
}

}

#endif /* end of include guard: FANMATRIX_H */
