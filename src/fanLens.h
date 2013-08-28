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

    void computeLensSpace();
private:
};

}

#endif /* end of include guard: FANLENS_H */
