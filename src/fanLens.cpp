#include "fanLens.h"

namespace fan {

fanLens::~fanLens() {
}

void fanLens::computeLensSpace( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up ) {
    fanVector3<float> look = lookAt - pos;
    fanVector3<float> toUp = up - pos;
    fanVector3<float> z = normalize( look );
    fanVector3<float> y = toUp - ( look * (look * toUp ) );
    fanVector3<float> x = cross( z, y );
    mLensSpace[0] = x;
    mLensSpace[1] = y;
    mLensSpace[2] = z;
}

}

