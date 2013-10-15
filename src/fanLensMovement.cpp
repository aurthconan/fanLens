#include "fanLensMovement.h"

#include <cmath>

#include <fanVector.h>
#include <fanMatrix.h>
#include <fanLens.h>

namespace fan {
/*
 *                 ^
 *           ^ y  /
 *           |   /
 *    ---------------
 *   |       | / z   |
 *   |       |/      |
 *   |       .-------|--> x
 *   |               |
 *   |               |
 *    ---------------
 */

#define SIN_COS( radian ) \
    float cosA = std::cos( radian ); \
    float sinA = std::sin( radian );

inline void RotateUseLensPos( const fanMatrix<float, 3, 3> rotation,
                              fanLens& lens ) {
    fanMatrix<float, 3, 3> transposeLens = transpose(lens.mLensSpace);

    fanVector<float, 3> lookAtVector = lens.mLookAt - lens.mPos;
    lookAtVector = lens.mLensSpace * lookAtVector;
    lookAtVector = rotation * lookAtVector;
    lookAtVector = transposeLens * lookAtVector;
    lens.mLookAt = lookAtVector + lens.mPos;

    fanVector<float, 3> upVector = lens.mUp - lens.mPos;
    upVector = lens.mLensSpace * upVector;
    upVector = rotation * upVector;
    upVector = transposeLens * upVector;
    lens.mUp = upVector + lens.mPos;

    lens.computeLensSpace( lens.mPos, lens.mLookAt, lens.mUp, lens.mDimens );
}

inline void RotateUseLookAtPos( const fanMatrix<float, 3, 3> rotation,
                                fanLens& lens ) {
    fanMatrix<float, 3, 3> transposeLens = transpose(lens.mLensSpace);

    fanVector<float, 3> posVector = lens.mPos - lens.mLookAt;
    posVector = lens.mLensSpace * posVector;
    posVector = rotation * posVector;
    posVector = transposeLens * posVector;
    lens.mPos = posVector + lens.mLookAt;

    fanVector<float, 3> upVector = lens.mUp - lens.mLookAt;
    upVector = lens.mLensSpace * upVector;
    upVector = rotation * upVector;
    upVector = transposeLens * upVector;
    lens.mUp = upVector + lens.mLookAt;

    lens.computeLensSpace( lens.mPos, lens.mLookAt, lens.mUp, lens.mDimens );
}

// rotate around y axis
void Yaw( float radian, fan::fanLens& lens ) {
    SIN_COS( radian );
    fanMatrix<float, 3, 3> rotation;
    rotation[0][0] = cosA; rotation[0][2] = sinA;
    rotation[1][1] = 1;
    rotation[2][0] = -sinA; rotation[2][2] = cosA;

    RotateUseLensPos( rotation, lens );
}

// rotate around x axis
void Pitch( float radian, fan::fanLens& lens ) {
    SIN_COS( radian );
    fanMatrix<float, 3, 3> rotation;
    rotation[0][0] = 1;
    rotation[1][1] = cosA; rotation[1][2] = -sinA;
    rotation[2][1] = sinA; rotation[2][2] = cosA;

    RotateUseLensPos( rotation, lens );
}

// rotate around z axis
void Roll( float radian, fan::fanLens& lens ) {
    SIN_COS( radian );
    fanMatrix<float, 3, 3> rotation;
    rotation[0][0] = cosA; rotation[0][1] = -sinA;
    rotation[1][0] = sinA; rotation[1][1] = cosA;
    rotation[2][2] = 1;

    RotateUseLensPos( rotation, lens );
}

void YawAroundFocus( float radian, fan::fanLens& lens ) {
    SIN_COS( radian );
    fanMatrix<float, 3, 3> rotation;
    rotation[0][0] = cosA; rotation[0][2] = sinA;
    rotation[1][1] = 1;
    rotation[2][0] = -sinA; rotation[2][2] = cosA;

    RotateUseLookAtPos( rotation, lens );
}

void PitchAroundFocus( float radian, fan::fanLens& lens ) {
    SIN_COS( radian );
    fanMatrix<float, 3, 3> rotation;
    rotation[0][0] = 1;
    rotation[1][1] = cosA; rotation[1][2] = -sinA;
    rotation[2][1] = sinA; rotation[2][2] = cosA;

    RotateUseLookAtPos( rotation, lens );
}

void RollAroundFocus( float radian, fan::fanLens& lens ) {
    SIN_COS( radian );
    fanMatrix<float, 3, 3> rotation;
    rotation[0][0] = cosA; rotation[0][1] = -sinA;
    rotation[1][0] = sinA; rotation[1][1] = cosA;
    rotation[2][2] = 1;

    RotateUseLookAtPos( rotation, lens );
}

void Translate( const fan::fanVector<float, 3>& pos, fan::fanLens& lens ) {
    fanMatrix<float, 3, 3> toWorldSpace;
    toWorldSpace = transpose( lens.mLensSpace );
    fan::fanVector3<float> inWorldSpace = toWorldSpace * pos;
    lens.mPos += inWorldSpace;
    lens.mLookAt += inWorldSpace;
    lens.mUp += inWorldSpace;
}

} /* namespace fan */

