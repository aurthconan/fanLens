#include "OrthogonalLens.h"

using namespace fan;

OrthogonalLens::OrthogonalLens( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up ) {
    computeLensSpace( pos, lookAt, up );
}

bool OrthogonalLens::projectToCameraSpace( fan::fanVector<int, 2>& pos,
                                           const fan::fanVector3<float>& world ) {
    fanVector3<float> vector = world - mPos;
    fanVector<int, 2> result;
    result[0] = mLensSpace[0] * vector;
    result[1] = mLensSpace[1] * vector;
    pos = result;
    return true;
}

