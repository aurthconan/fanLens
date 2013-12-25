#include "QuadTree.h"

#include <fanVector3.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>

#include <stdlib.h>
#include <time.h>

using namespace fan;
using namespace std;

QuadTree::QuadTree( const fanVector3<float>& a,
          const fanVector3<float>& b,
          const fanVector3<float>& c,
          const fanVector3<float>& d )
    : mThisTile( new fanTriangleMesh() )
{
    mThisTile->mVertices.reset( new fanBufferObject<fanVector3<float> >( 4 ) );
    mThisTile->mVertices->mBuffer[0] = a;
    mThisTile->mVertices->mBuffer[1] = b;
    mThisTile->mVertices->mBuffer[2] = c;
    mThisTile->mVertices->mBuffer[3] = d;

    mThisTile->mFaces.reset( new fanBufferObject<fanTriangle>( 2 ) );
    mThisTile->mFaces->mBuffer[0] = fanTriangle( &mThisTile->mVertices->mBuffer[0],
                                              &mThisTile->mVertices->mBuffer[1],
                                              &mThisTile->mVertices->mBuffer[2],
                                              0, 1, 2 );
    mThisTile->mFaces->mBuffer[1] = fanTriangle( &mThisTile->mVertices->mBuffer[1],
                                              &mThisTile->mVertices->mBuffer[3],
                                              &mThisTile->mVertices->mBuffer[2],
                                              1, 3, 2 );

    mThisTile->mNormals.reset( new fanBufferObject<fanVector3<float> >( 4 ) );
    // be lazy, just use (0,0,1) as normal
    mThisTile->mNormals->mBuffer[0] = fanVector3<float>( 0, 0, 1 );
    mThisTile->mNormals->mBuffer[1] = fanVector3<float>( 0, 0, 1 );
    mThisTile->mNormals->mBuffer[2] = fanVector3<float>( 0, 0, 1 );
    mThisTile->mNormals->mBuffer[3] = fanVector3<float>( 0, 0, 1 );
}

void QuadTree::generateTerrian( TriangleMeshObject& terrian,
                                // fan::fanVector3<float>& eyePos ) {
                                size_t level ) {
    if ( mThisTile.get() == NULL ) {
        return;
    }

    // compute center
    fanVector3<float> center;
    for ( size_t i = 0, max = mThisTile->mFaces->mSize;
            i < max; ++i ) {
        center += mThisTile->mFaces->mBuffer[i].mCenter;
    }
    center /= mThisTile->mFaces->mSize;

    // compute the distance to judge if we need to subdivide
    if ( level == 0 /*lengthSquare( eyePos - center ) > 1000*/ ) {
        terrian.mMeshes.push_back( mThisTile );
    } else {
        if ( mChildTiles[0][0].get() == NULL ) {
            // subdivide
            // 2 e 3
            // b c d
            // 0 a 1
#define RANDOM_Z_AXIS( VARIABLE_NAME, ONE_END, ANOTHER_END )                \
            fanVector3<float> VARIABLE_NAME = (ONE_END + ANOTHER_END)/2.0f;
            /*
            VARIABLE_NAME.z = (((float)(rand()%100))/100.0f)                \
                                * (ANOTHER_END.z-ONE_END.z)                 \
                                + ONE_END.z;
                                */

            RANDOM_Z_AXIS( a, mThisTile->mVertices->mBuffer[0], mThisTile->mVertices->mBuffer[1] );
            RANDOM_Z_AXIS( b, mThisTile->mVertices->mBuffer[0], mThisTile->mVertices->mBuffer[2] );
            RANDOM_Z_AXIS( d, mThisTile->mVertices->mBuffer[1], mThisTile->mVertices->mBuffer[3] );
            RANDOM_Z_AXIS( e, mThisTile->mVertices->mBuffer[2], mThisTile->mVertices->mBuffer[3] );
            RANDOM_Z_AXIS( c, b, d );
            c.z = (((float)(rand()%100))/100.0f)
                      * (d.z-b.z) * 2
                      + b.z;

            // 2   e   3
            //  1,0 1,1
            // b   c   d
            //  0,0 0,1
            // 0   a   1
            mChildTiles[0][0].reset( new QuadTree( mThisTile->mVertices->mBuffer[0], a,
                                                   b, c ) );
            mChildTiles[0][1].reset( new QuadTree( a, mThisTile->mVertices->mBuffer[1],
                                                   c, d ) );
            mChildTiles[1][0].reset( new QuadTree( b, c,
                                                   mThisTile->mVertices->mBuffer[2], e ) );
            mChildTiles[1][1].reset( new QuadTree( c, d,
                                                   e, mThisTile->mVertices->mBuffer[3] ) );
        }
        --level;
        mChildTiles[0][0]->generateTerrian( terrian, level );// eyePos );
        mChildTiles[0][1]->generateTerrian( terrian, level );// eyePos );
        mChildTiles[1][0]->generateTerrian( terrian, level );// eyePos );
        mChildTiles[1][1]->generateTerrian( terrian, level );// eyePos );
    }
}

