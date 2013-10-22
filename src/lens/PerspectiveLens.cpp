#include "PerspectiveLens.h"

#include <fanTriangle.h>

PerspectiveLens::PerspectiveLens( fan::fanVector3<float> pos,
                                  fan::fanVector3<float> lookAt,
                                  fan::fanVector3<float> up,
                                  fan::fanVector3<float> dimens,
                                  float distance )
    : mDistance( distance ) {
    computeLensSpace( pos, lookAt, up, dimens );
}

bool PerspectiveLens::cullFace( const fan::fanTriangle& triangle,
                      const fan::fanMatrix<float, 4, 4>& worldTransformation ) {
    fan::fanVector3<float> view = normalize(mPos - mLookAt);
    fan::fanVector3<float> viewPos = mPos + view * ( mDistance );
    fan::fanVector<float, 4> homoPos = triangle.mCenter;
    homoPos[3] = 1.0f;
    homoPos = worldTransformation * homoPos;
    fan::fanVector3<float> viewToPos = homoPos - viewPos;
    return dot( triangle.mNormal, viewToPos ) <= 0;
}

fan::fanMatrix<float, 4, 4> PerspectiveLens::getTransformation() {
    fan::fanMatrix<float, 4, 4> result;
    result[0][0] = 1;
    result[1][1] = 1;
    result[2][2] = 1;
    result[3][3] = 1;
    result[3][2] = 1/mDistance;
    return result;
}
