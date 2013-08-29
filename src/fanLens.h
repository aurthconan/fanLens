#ifndef FANLENS_H

#define FANLENS_H

#include "fanVector3.h"
#include "fanMatrix.h"

namespace fan {

/* A "Camera" class if in other renderer*/
class fanLens
{
public:
    fanMatrix<float, 3, 3> mLensSpace;

    virtual ~fanLens();

    void computeLensSpace( fanVector3<float> pos,
                           fanVector3<float> lookAt,
                           fanVector3<float> up );
};

}

#endif /* end of include guard: FANLENS_H */
