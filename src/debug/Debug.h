#ifndef DEBUG_H

#define DEBUG_H

#include <fanVector.h>
#include <fanMatrix.h>

#include <iostream>

template<class charT, class Traits, typename T, size_t DIMENS>
std::basic_ostream<charT, Traits>&
operator << (std::basic_ostream<charT, Traits >& os, const fan::fanVector<T, DIMENS>& vector)
{
    if ( DIMENS >= 1 ) {
        os << vector.axis[0];
    }
    for ( size_t i = 1; i < DIMENS; ++i ) {
        os << "," << vector.axis[i];
    }
    return os;
}

template<class charT, class Traits, typename T, size_t R, size_t C>
std::basic_ostream<charT, Traits>&
operator << (std::basic_ostream<charT, Traits >& os, const fan::fanMatrix<T, R, C>& matrix)
{
    os << "[";
    for ( size_t i = 0; i < R; ++i ) {
        os << matrix.rows[i] << ";";
    }
    os << "]";
    return os;
}

#endif /* end of include guard: DEBUG_H */
