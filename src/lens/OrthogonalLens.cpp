#include "OrthogonalLens.h"

using namespace fan;

OrthogonalLens::OrthogonalLens( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up ) {
    computeLensSpace( pos, lookAt, up );
}

bool OrthogonalLens::projectInCameraSpace( fan::fanVector<int, 2>& pos,
                                           const fan::fanVector3<float>& camera ) {
    pos[0] = camera[0];
    pos[1] = camera[1];
    return true;
}

