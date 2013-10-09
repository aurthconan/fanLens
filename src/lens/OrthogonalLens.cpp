#include "OrthogonalLens.h"

#include <fanTriangle.h>

using namespace fan;

OrthogonalLens::OrthogonalLens( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up,
                                fanVector3<float> dimens ) {
    computeLensSpace( pos, lookAt, up, dimens );
}

bool OrthogonalLens::cullFace( const fanTriangle& triangle ) {
    fanVector3<float> view = mLookAt - mPos;
    return dot( triangle.mNormal, view ) <= 0;
}

fanMatrix<float, 4, 4> OrthogonalLens::getTransformation() {
    fanMatrix<float, 4, 4> result;
    result[0][0] = 1;
    result[1][1] = 1;
    result[2][2] = 1;
    result[3][3] = 1;
    return result;
}

