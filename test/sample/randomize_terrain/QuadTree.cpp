#include "QuadTree.h"

#include <fanVector3.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>

#include <stdlib.h>
#include <time.h>

#include "PointGenerator.h"

using namespace fan;
using namespace std;

QuadTree::QuadTree( const fanVector3<float>& a,
          const fanVector3<float>& b,
          const fanVector3<float>& c,
          const fanVector3<float>& d )
    : mLevel( 0 )
    , mIndex{ 0, 0 }
    , mPointGenerator( new PointGenerator( 0, NULL, a, b, c, d ) )
    , mThisTile( new fanTriangleMesh() )
{
    initThisTile();
}

QuadTree::QuadTree( size_t level, fan::fanVector<size_t, 2> index,
                    PointGenerator* generator )
    : mLevel( level )
    , mIndex( index )
    , mPointGenerator( generator )
    , mThisTile( new fanTriangleMesh() )
{
    initThisTile();
}

QuadTree::~QuadTree()
{
    if ( mLevel == 0 && mPointGenerator ) {
        delete mPointGenerator;
    }
}

void QuadTree::initThisTile()
{
    fanVector3<float> a =
        mPointGenerator->getPoint( PointGenerator::PointIndex( mLevel, mIndex[0], mIndex[1] ) );
    fanVector3<float> b =
        mPointGenerator->getPoint( PointGenerator::PointIndex( mLevel, mIndex[0]+1, mIndex[1] ) );
    fanVector3<float> c =
        mPointGenerator->getPoint( PointGenerator::PointIndex( mLevel, mIndex[0], mIndex[1]+1 ) );
    fanVector3<float> d =
        mPointGenerator->getPoint( PointGenerator::PointIndex( mLevel, mIndex[0]+1, mIndex[1]+1 ) );

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
            size_t newLevel = mLevel+1;
            fanVector<size_t, 2> newIndex = mIndex * 2;
            mChildTiles[0][0].reset( new QuadTree( newLevel,
                                                   newIndex,
                                                   mPointGenerator ) );
            mChildTiles[0][1].reset( new QuadTree( newLevel,
                                                   newIndex + fanVector<size_t,2>{0,1},
                                                   mPointGenerator ) );
            mChildTiles[1][0].reset( new QuadTree( newLevel,
                                                   newIndex + fanVector<size_t,2>{1,0},
                                                   mPointGenerator ) );
            mChildTiles[1][1].reset( new QuadTree( newLevel,
                                                   newIndex + fanVector<size_t,2>{1,1},
                                                   mPointGenerator ) );
        }
        --level;
        mChildTiles[0][0]->generateTerrian( terrian, level );// eyePos );
        mChildTiles[0][1]->generateTerrian( terrian, level );// eyePos );
        mChildTiles[1][0]->generateTerrian( terrian, level );// eyePos );
        mChildTiles[1][1]->generateTerrian( terrian, level );// eyePos );
    }
}

