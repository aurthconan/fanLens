#include "DepthCamera.h"

#include <algo/rasterize/fanScanLineGenerator.h>

#include <texture/MemoryTexture.h>

#include <camera/RasterisationScanner.h>
#include <filler/DepthFiller.h>

using namespace fan;

class DepthPixelConvertor
    : public fanTexture<int, float, 2>
{
public:
    DepthPixelConvertor( fanTexture<int, float, 2>& zBuffer,
                         fanTexture<int, fanPixel, 2>& pixelTexture )
        : fanTexture<int, float, 2>( zBuffer.getDimens() )
        , mZBuffer( zBuffer )
        , mPixelTexture( pixelTexture )
    {
    }

    float getValue( fanVector<int, 2> index ) const
    {
        return mZBuffer.getValue( index );
    }

    void setValue( const fanVector<int, 2>& index,
                   const float& value )
    {
        if ( value < 0 || value > 1 ) {
            return;
        }
        mZBuffer.setValue( index, value );
        fanPixel pixel( 255, 255*value,
                             255*value,
                             255*value );
        mPixelTexture.setValue( index, pixel );
    }

    fanTexture<int, float, 2>& mZBuffer;
    fanTexture<int, fanPixel, 2>& mPixelTexture;
};

void DepthCamera::takePicture( fan::fanScene& scene,
                               fan::fanTexture<int, fanPixel, 2>& film,
                               fan::fanLens& lens )
{
    fan::fanVector<int, 2> dimens = film.getDimens();

    MemoryTexture<int, float, 2> zBuffer( dimens );
    zBuffer.reset( 2.0f );
    DepthPixelConvertor convertor( zBuffer, film );
    DepthFiller filler;
    RasterisationScanner<DepthFiller, float> depthFiller( filler );
    depthFiller.takePicture( scene, convertor, lens );
}

