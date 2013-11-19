#include "ShadowUsingZBufferLight.h"

#include <camera/RasterisationScanner.h>
#include <filler/DepthFiller.h>

#include <texture/film/FreeImageFilm.h>
#include <texture/LinearInterpolatorTexture2D.h>

#include <limits>
#include <cmath>

using namespace boost;
using namespace fan;

#define _BUFFER_SIZE_ 400

ShadowUsingZBufferLight::ShadowUsingZBufferLight( shared_ptr<PointLight> point,
                                                  const fanVector<float, 3>& dimens,
                                                  const fanVector<float, 3>& pointAt )
    : mpPointLight( point )
    , mDimens{_BUFFER_SIZE_, _BUFFER_SIZE_}
    , m3Dimens( dimens )
    , mPointAt( pointAt )
{
    zNegative.reset( new MemoryTexture<int, float, 2>( mDimens ) );
    zNegative->reset(std::numeric_limits<float>::max());
}

class ShadowZDepthFilter
    : public fan::fanTexture<int, float, 2>
{
public:
    ShadowZDepthFilter( fan::fanTexture<int, float, 2>& buffer )
        : fanTexture<int, float, 2>( buffer.getDimens() )
        , mBuffer( buffer )
    {
    }

    virtual float getValue( fanVector<int, 2> index ) const {
        return mBuffer.getValue( index );
    }

    virtual void setValue( const fanVector<int, 2>& index,
                           const float& value ) {
        mBuffer.setValue( index, value );
    }

    fan::fanTexture<int, float, 2>& mBuffer;
};

void ShadowUsingZBufferLight::bakeShadowMap( fanScene& scene )
{
    zNLens.reset( new PerspectiveLens(mpPointLight->mPos, mPointAt,
                            mpPointLight->mPos + fanVector3<float>(1,0,0), m3Dimens, _BUFFER_SIZE_/2));

    zNegative->reset(std::numeric_limits<float>::max());
    RasterisationScanner<DepthFiller, float> depthFiller;
    ShadowZDepthFilter zN( *zNegative );

    depthFiller.takePicture( scene, zN, *zNLens );

    /*
    FreeImageFilm freeImage( mDimens, "zNShadowMap.png" );
    fanVector<int, 2> pos;
    for ( int i = 0; i < mDimens[1]; ++i ) {
        pos[1] = i;
        for ( int j = 0; j < mDimens[0]; ++j ) {
            pos[0] = j;
            float depth = (1.0f + zNegative->getValue( pos ))/2.0f;
            fanPixel pixel( 255, 255*depth, 255*depth, 255*depth );
            freeImage.setValue( pos, pixel );
        }
    }
    freeImage.develope();
    */

}

fanPixel ShadowUsingZBufferLight::getLight( fanVector3<float> world,
                                            fanVector3<float> normal,
                                            fanVector3<float> viewer )
{
    fan::fanVector<float, 2> pos;
    fan::fanVector<float, 4> homoPos;
    //LinearInterpolatorTexture2D<float> texture( *zNegative );
    project( world, *zNLens, mDimens, pos, homoPos );
    if ( !(homoPos[0] < 0 || homoPos[0] > 1
           || homoPos[1] < 0 || homoPos[1] > 1) ) {
        if ( std::abs( homoPos[2] - zNegative->getValue( pos ) ) <= 0.002 ) {
            return mpPointLight->getLight( world, normal, viewer );
        } else {
            return fanPixel(0,0,0,0);
        }
    }

    return fanPixel(0,0,0,0);
}

