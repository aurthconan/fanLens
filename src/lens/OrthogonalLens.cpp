#include "OrthogonalLens.h"

using namespace fan;

OrthogonalLens::OrthogonalLens( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up ) {
    computeLensSpace( pos, lookAt, up );
}

fanVector<int, 2> OrthogonalLens::project( const fanVector3<float>& world,
                                           const fanTexture<int, 2>& space ) {
    fanVector3<float> vector = world - mPos;
    fanVector<int, 2> result;
    result[0] = mLensSpace[0] * vector;
    result[1] = mLensSpace[1] * vector;
    return result;
}

