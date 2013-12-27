#include "PointGenerator.h"

#include <cmath>
#include <algorithm>

#include <stdio.h>

using namespace std;
using namespace fan;

PointGenerator::PointIndex::PointIndex( size_t level, size_t x, size_t y )
    : mConvertedLevel( 0 )
    , mConvertedX( 0 )
    , mConvertedY( 0 )
    , mOriginalLevel( level )
    , mOriginalX( x )
    , mOriginalY( y )
{
    size_t l = std::pow( 2, level ) + 1;
    if ( mOriginalX >= l || mOriginalY >= l )
        return;

    mConvertedLevel = mOriginalLevel;
    mConvertedX = mOriginalX;
    mConvertedY = mOriginalY;
    while ( !(mConvertedX & 0x1) && !(mConvertedY & 0x1) && mConvertedLevel != 0 ) {
        mConvertedX = mConvertedX >> 1;
        mConvertedY = mConvertedY >> 1;
        --mConvertedLevel;
    }
}

PointGenerator::PointGenerator( size_t level,
                                PointGenerator* parent,
                                fan::fanVector3<float> a,
                                fan::fanVector3<float> b,
                                fan::fanVector3<float> c,
                                fan::fanVector3<float> d )
    : mLevel( level )
    , mHigherLevel( parent )
    , mLowerLevel( NULL )
    , mPoints( 3*pow(2,2*mLevel)+pow(2,mLevel+1) )
    , mGenerated( 3*pow(2,2*mLevel)+pow(2,mLevel+1) )
{
    if ( mLevel == 0 ) {
        for ( size_t i = 0, max = mGenerated.capacity();
                i < max; ++i ) {
            mGenerated[i] = true;
        }
        mPoints[0] = a; mPoints[1] = b;
        mPoints[2] = c; mPoints[3] = d;
    } else {
        for ( size_t i = 0, max = mGenerated.capacity();
                i < max; ++i ) {
            mGenerated[i] = false;
        }
    }
}

PointGenerator::~PointGenerator()
{
    if ( mLowerLevel ) {
        delete mLowerLevel;
    }
}

fan::fanVector3<float> PointGenerator::getPoint( const PointIndex& index )
{

#define AVERAGE_AND_ADD_RANDOM(OUTPUT, A, B, C, D, SIZE)\
    OUTPUT = (A+B+C+D)/SIZE; \
    OUTPUT.z =  (max(max(max(A.z,B.z),C.z),D.z) \
               - min(min(min(A.z,B.z),C.z),D.z))*(((float)(rand()%100))/100.0f) \
               + min(min(min(A.z,B.z),C.z),D.z);

    if ( index.mConvertedLevel == mLevel ) {
        size_t pointIndex = computeIndex( index );
        if ( mGenerated[pointIndex] == false ) {
            float neighborNum = 4;
            if ( index.mConvertedX & 0x1 && index.mConvertedY & 0x1 ) {
                // c * d
                // * x *
                // a * b
                fanVector3<float> a = getPoint( PointIndex(mLevel, index.mConvertedX-1, index.mConvertedY-1) );
                fanVector3<float> b = getPoint( PointIndex(mLevel, index.mConvertedX+1, index.mConvertedY-1) );
                fanVector3<float> c = getPoint( PointIndex(mLevel, index.mConvertedX-1, index.mConvertedY+1) );
                fanVector3<float> d = getPoint( PointIndex(mLevel, index.mConvertedX+1, index.mConvertedY+1) );
                AVERAGE_AND_ADD_RANDOM( mPoints[pointIndex], a, b, c, d, neighborNum );
            } else {
                // * d *
                // b x c
                // * a *
                fanVector3<float> a;
                fanVector3<float> b;
                fanVector3<float> c;
                fanVector3<float> d;
                if ( index.mConvertedY > 0 && ( index.mConvertedY < pow( 2, mLevel ) ) ) {
                    a = getPoint( PointIndex( mLevel, index.mConvertedX, index.mConvertedY-1 ) );
                    d = getPoint( PointIndex( mLevel, index.mConvertedX, index.mConvertedY+1 ) );
                } else {
                    neighborNum -= 2;
                }

                if ( index.mConvertedX > 0 && ( index.mConvertedX < pow( 2, mLevel ) ) ) {
                    b = getPoint( PointIndex( mLevel, index.mConvertedX-1, index.mConvertedY ) );
                    c = getPoint( PointIndex( mLevel, index.mConvertedX+1, index.mConvertedY ) );
                } else {
                    neighborNum -= 2;
                }
                AVERAGE_AND_ADD_RANDOM( mPoints[pointIndex], a, b, c, d, neighborNum );
            }
            mGenerated[pointIndex] = true;
        }
        return mPoints[pointIndex];
    } else if ( index.mConvertedLevel > mLevel ) {
        if ( mLowerLevel == NULL ) {
            mLowerLevel = new PointGenerator( mLevel+1, this );
        }
        return mLowerLevel->getPoint( index );
    } else {
        if ( mHigherLevel == NULL ) {
            return fanVector3<float>();
        }
        return mHigherLevel->getPoint( index );
    }
    return fanVector3<float>();
}

size_t PointGenerator::computeIndex( const PointIndex& index )
{
    size_t result = 0;
    if ( index.mConvertedLevel == 0 ) {
        return index.mConvertedY*2 + index.mConvertedX;
    }
    size_t odd = pow( 2, index.mConvertedLevel-1 );
    size_t even = pow( 2, index.mConvertedLevel ) + 1;

    size_t oddLineNum = 0;
    size_t evenLineNum = 0;
    if ( index.mConvertedY > 0 ) {
        evenLineNum = (index.mConvertedY)/2;
        oddLineNum = (index.mConvertedY+1)/2;
    }
    result = oddLineNum*odd + evenLineNum*even;

    if ( index.mConvertedY & 0x1 ) {  // odd line
        result += index.mConvertedX;
    } else {  // even line
        result += index.mConvertedX/2;
    }
    return result;
}

