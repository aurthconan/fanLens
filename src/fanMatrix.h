#ifndef FANMATRIX_H

#define FANMATRIX_H

#include "fanVector.h"

namespace fan {

template<typename T, size_t ROWS, size_t COLS>
class fanMatrix
{
public:
    static const size_t rowsNum = ROWS;
    static const size_t colsNum = COLS;
    union {
        fanVector<T, COLS> rows[ROWS];
    };

    fanMatrix();
    ~fanMatrix();

};

}

#endif /* end of include guard: FANMATRIX_H */
