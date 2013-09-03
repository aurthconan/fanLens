#include "PerspectiveLens.h"

PerspectiveLens::PerspectiveLens( fan::fanVector3<float> pos,
                                  fan::fanVector3<float> lookAt,
                                  fan::fanVector3<float> up,
                                  float distance )
    : mDistance( distance ) {
    computeLensSpace( pos, lookAt, up );
}

bool PerspectiveLens::projectInCameraSpace( fan::fanVector<int, 2>& pos,
                                       const fan::fanVector3<float>& camera ) {
    pos[0] = (mDistance*camera[0])/(mDistance+camera[2]);
    pos[1] = (mDistance*camera[1])/(mDistance+camera[2]);
    return true;
}

