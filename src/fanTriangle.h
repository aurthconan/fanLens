#ifndef FANTRIANGLE_H

#define FANTRIANGLE_H

#include "fanVector3.h"
#include <cstddef>

namespace fan {

class fanTriangle
{
public:
    fanTriangle();
    fanTriangle( fanVector3<float>* _a,
                 fanVector3<float>* _b,
                 fanVector3<float>* _c,
                 size_t _aIndex,
                 size_t _bIndex,
                 size_t _cIndex,
                 fanVector3<float> center = fanVector3<float>(),
                 fanVector3<float> normal = fanVector3<float>() );

    fanVector3<float>* a;
    fanVector3<float>* b;
    fanVector3<float>* c;

    size_t aIndex, bIndex, cIndex;

    fanVector3<float> mCenter;
    fanVector3<float> mNormal;
};

}

#endif /* end of include guard: FANTRIANGLE_H */
