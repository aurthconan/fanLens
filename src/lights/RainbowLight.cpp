#include "RainbowLight.h"

#include <utils/RGBConverter.h>

RainbowLight::RainbowLight( fan::fanVector3<float> pos )
    : PointLight( pos, fan::fanPixel( 0, 0, 0, 0 ) )
{
}

fan::fanPixel RainbowLight::getLight( fan::fanVector3<float> world,
                                      fan::fanVector3<float> normal,
                                      fan::fanVector3<float> viewer )
{
    (void) viewer;
    fan::fanVector3<float> lightToPoint = mPos - world;

    float r, g, b;
    r = g = b = 0;
    RGBConverter::fromHSVToRGB( lightToPoint.length()*360, 0.5, 0.5, r, g, b );
    fan::fanPixel pixel = fan::fanPixel( 255, r*255, g*255, b*255 );

    float ratio = normalize(lightToPoint) * normal;
    if ( ratio < 0 ) {
        return fan::fanPixel( 255, 0, 0, 0 );
    }
    return pixel * ratio;

}

