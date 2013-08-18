#ifndef FANVECTOR_H

#define FANVECTOR_H

#include "fanTriple.h"

template<typename T>
class fanVector
    : public fanTriple<T>
{
public:
    fanVector();
    ~fanVector();

    fanVector( T value );

private:
    /* data */
};

#endif /* end of include guard: FANVECTOR_H */

