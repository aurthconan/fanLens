#include "fanLens.h"

namespace fan {

fanLens::~fanLens() {
}

void fanLens::computeLensSpace( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up ) {
    mPos = pos;
    mLookAt = lookAt;
    mUp = up;
    fanVector3<float> look = lookAt - pos;
    fanVector3<float> toUp = up - pos;
    fanVector3<float> z = normalize( look );
    /*
     *       up
     *       ^^
     * toUp / |
     *     /  |  y
     *    /   |
     *  pos--->-------->lookAt
     *      a      look
     */
    float lookLength = length(look);
    float v1 = ((look*toUp)/(lookLength*lookLength));
    float v2 = look*toUp;
    fanVector3<float> a = look * ((look*toUp)/(lookLength*lookLength));
    fanVector3<float> y = normalize(toUp - a);
    fanVector3<float> x = normalize(cross( y, z ));
    mLensSpace[0] = x;
    mLensSpace[1] = y;
    mLensSpace[2] = z;
}

fanVector<int, 2> fanLens::project( const fanVector3<float>& world,
                                    const fanTexture<int, 2>& space ) {
    (void) world; (void) space;
    return fanVector<int, 2>();
}

}

