#include "PerspectiveLens.h"

PerspectiveLens::PerspectiveLens( fan::fanVector3<float> pos,
                                  fan::fanVector3<float> lookAt,
                                  fan::fanVector3<float> up,
                                  float distance )
    : mDistance( distance ) {
    computeLensSpace( pos, lookAt, up );
}

bool PerspectiveLens::projectInCameraSpace( fan::fanVector<float, 2>& pos,
                                       const fan::fanVector3<float>& camera ) {
    pos[0] = (mDistance*camera[0])/(mDistance+camera[2]);
    pos[1] = (mDistance*camera[1])/(mDistance+camera[2]);
    return true;
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
