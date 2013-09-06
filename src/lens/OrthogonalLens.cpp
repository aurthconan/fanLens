#include "OrthogonalLens.h"

using namespace fan;

OrthogonalLens::OrthogonalLens( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up ) {
    computeLensSpace( pos, lookAt, up );
}

bool OrthogonalLens::projectInCameraSpace( fan::fanVector<float, 2>& pos,
                                           const fan::fanVector3<float>& camera ) {
    pos[0] = camera[0];
    pos[1] = camera[1];
    return true;
}

fanMatrix<float, 4, 4> OrthogonalLens::getTransformation() {
    fanMatrix<float, 4, 4> result;
    result[0][0] = 1;
    result[1][1] = 1;
    result[2][2] = 1;
    result[3][3] = 1;
    return result;
}

