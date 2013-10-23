#include "PointLight.h"

PointLight::PointLight( fan::fanVector3<float> pos,
                        fan::fanPixel light )
    : mPos( pos )
    , mLightPixel( light ) {
}

fan::fanPixel PointLight::getLight( fan::fanVector3<float> world,
                                    fan::fanVector3<float> normal,
                                    fan::fanVector3<float> viewer ) {
    (void) viewer;
    fan::fanVector3<float> lightToPoint = mPos - world;
    float ratio = normalize(lightToPoint) * normal;
    if ( ratio < 0 ) {
        return fan::fanPixel( 255, 0, 0, 0 );
    }
    return mLightPixel * ratio;
}

