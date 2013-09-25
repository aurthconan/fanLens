#include "PerspectiveLens.h"

PerspectiveLens::PerspectiveLens( fan::fanVector3<float> pos,
                                  fan::fanVector3<float> lookAt,
                                  fan::fanVector3<float> up,
                                  fan::fanVector3<float> dimens,
                                  float distance )
    : mDistance( distance ) {
    computeLensSpace( pos, lookAt, up, dimens );
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
