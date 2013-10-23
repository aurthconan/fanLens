#include "Ambient.h"

Ambient::Ambient( fan::fanPixel pixel )
    : mPixel( pixel )
{
}

fan::fanPixel Ambient::getLight( fan::fanVector3<float> world,
                                 fan::fanVector3<float> normal,
                                 fan::fanVector3<float> viewer ) {
    (void) world; (void) normal; (void) viewer;
    return mPixel;
}

