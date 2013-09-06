#ifndef FANLENS_H

#define FANLENS_H

#include "fanVector3.h"
#include "fanMatrix.h"
#include "fanTexture.h"

namespace fan {

/* A "Camera" class if in other renderer*/
class fanLens
{
public:
    fanMatrix<float, 3, 3> mLensSpace;
    fanMatrix<float, 4, 4> mViewTransformation;
    fanVector3<float> mPos;
    fanVector3<float> mLookAt;
    fanVector3<float> mUp;

    fanLens();
    virtual ~fanLens();

    bool project( fanVector<float, 2>& pos,
                  const fanVector3<float>& world,
                  const fanTexture<int, 2>& space );

    void move( fanVector<float, 3> move );

protected:
    virtual bool projectInCameraSpace( fanVector<float, 2>& pos,
                                       const fanVector3<float>& camera );

    void computeLensSpace( fanVector3<float> pos,
                           fanVector3<float> lookAt,
                           fanVector3<float> up );

    virtual fan::fanMatrix<float, 4, 4> getTransformation();
};

}

#endif /* end of include guard: FANLENS_H */
