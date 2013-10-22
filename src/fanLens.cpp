#include "fanLens.h"

namespace fan {

fanLens::fanLens()
    : mLensSpace()
    , mViewTransformation()
    , mPos()
    , mLookAt()
    , mUp() {
}

fanLens::~fanLens() {
}

void fanLens::computeLensSpace( fanVector3<float> pos,
                                fanVector3<float> lookAt,
                                fanVector3<float> up,
                                fanVector3<float> dimens ) {
    mPos = pos;
    mLookAt = lookAt;
    mUp = up;
    mDimens = dimens;
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
    float lookLengthSqu = lengthSquare(look);
    fanVector3<float> a = look * ((look*toUp)/(lookLengthSqu));
    fanVector3<float> y = normalize(toUp - a);
    fanVector3<float> x = normalize(cross( z, y ));
    mLensSpace[0] = x;
    mLensSpace[1] = y;
    mLensSpace[2] = z;

    fanMatrix<float, 4, 4> childTransform = getTransformation();
    mViewTransformation = mLensSpace;
    mViewTransformation[3][3] = 1;
    mViewTransformation = childTransform * mViewTransformation;

    fanMatrix<float, 4, 4> unitBox;
    unitBox[0][0] = 1/mDimens[0]; unitBox[0][3] = 0.5f;
    unitBox[1][1] = 1/mDimens[1]; unitBox[1][3] = 0.5f;
    unitBox[2][2] = 1/mDimens[2];
    unitBox[3][3] = 1;

    mViewTransformation = unitBox * mViewTransformation;
}

fanMatrix<float, 4, 4> fanLens::getTransformation() {
    return fanMatrix<float, 4, 4>();
}

void fanLens::move( fanVector<float, 3> move ) {
    computeLensSpace( mPos + move,
                      mLookAt + move,
                      mUp + move,
                      mDimens );
}

bool fanLens::cullFace( const fan::fanTriangle& triangle,
                        const fanMatrix<float, 4, 4>& worldTransformation ) {
    (void) triangle; (void) worldTransformation;
    return false;
}

}

