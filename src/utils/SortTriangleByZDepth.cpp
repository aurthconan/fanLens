#include "SortTriangleByZDepth.h"

#include <fanLens.h>
#include <fanTriangle.h>

using namespace fan;

SortTriangleByZDepth::SortTriangleByZDepth( fanLens& lens )
    : mLens( lens ) {
}

bool SortTriangleByZDepth::operator()( const fan::fanTriangle& l,
                                       const fan::fanTriangle& r ) {
    fanVector<float, 4> homoL = l.mCenter - mLens.mPos;
    homoL[3] = 1.0f;
    homoL = mLens.mViewTransformation * homoL;

    fanVector<float, 4> homoR = r.mCenter - mLens.mPos;
    homoR[3] = 1.0f;
    homoR = mLens.mViewTransformation * homoR;

    return homoL[2] < homoR[2];
}

