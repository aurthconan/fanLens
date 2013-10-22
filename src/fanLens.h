#ifndef FANLENS_H

#define FANLENS_H

#include "fanVector3.h"
#include "fanMatrix.h"
#include "fanTexture.h"

namespace fan {

class fanTriangle;

/* A "Camera" class if in other renderer*/
class fanLens
{
public:
    fanMatrix<float, 3, 3> mLensSpace;
    fanMatrix<float, 4, 4> mViewTransformation;
    fanVector3<float> mPos;
    fanVector3<float> mLookAt;
    fanVector3<float> mUp;
    fanVector3<float> mDimens;

    fanLens();
    virtual ~fanLens();

    void move( fanVector<float, 3> move );

    virtual bool cullFace( const fanTriangle& triangle,
                           const fanMatrix<float, 4, 4>& worldTransformation);

    void computeLensSpace( fanVector3<float> pos,
                           fanVector3<float> lookAt,
                           fanVector3<float> up,
                           fanVector3<float> dimens );

protected:

    virtual fan::fanMatrix<float, 4, 4> getTransformation();
};

}

#endif /* end of include guard: FANLENS_H */
