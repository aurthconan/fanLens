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
    fanVector3<float> a = look * ((look*toUp)/(lookLength*lookLength));
    fanVector3<float> y = normalize(toUp - a);
    fanVector3<float> x = normalize(cross( y, z ));
    mLensSpace[0] = x;
    mLensSpace[1] = y;
    mLensSpace[2] = z;
}

bool fanLens::project( fanVector<int, 2>& pos,
                       const fanVector3<float>& world,
                       const fanTexture<int, 2>& space ) {
    bool visible = true;
    fanVector<int, 2> result;
    fanVector3<float> vector = world - mPos;
    vector = mLensSpace * vector;
    if ( !projectInCameraSpace( result, vector ) ) {
        visible = false;
    }

    // a viewport transformation, need a sampler class to do this
    fanVector<int, 2> dimens = space.getDimens();
    int xMax = dimens[0]/2; int yMax = dimens[1]/2;
    int xModify = dimens[0]%2; int yModify = dimens[1]%2;
    if ( result[0] > xMax + xModify || result[0] < -xMax
            || result[1] > yMax + yModify || result[1] < -yMax ) {
        visible = false;
    }

    pos = result;
    pos[0] += xMax; pos[1] += yMax;
    return visible;
}

bool fanLens::projectInCameraSpace( fanVector<int, 2>& pos,
                                    const fanVector3<float>& world ) {
    (void)pos; (void)world;
    return false;
}

void fanLens::move( fanVector<float, 3> move ) {
    computeLensSpace( mPos + move,
                      mLookAt + move,
                      mUp + move );
}

}

