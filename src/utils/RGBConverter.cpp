#include "RGBConverter.h"
#include <cmath>

void RGBConverter::fromHSVToRGB( const float hue_angle,
                                 const float saturation,
                                 const float value,
                                 float& R,
                                 float& G,
                                 float& B )
{
    float _h = (hue_angle)/360.0f;
    int i = int(_h*6);
    float f = _h * 6 - i;
    float p = value * ( 1 - saturation );
    float q = value * ( 1 - f * saturation );
    float t = value * ( 1 - ( 1 - f ) * saturation );

    switch(i % 6){
        case 0: R = value, G = t, B = p; break;
        case 1: R = q, G = value, B = p; break;
        case 2: R = p, G = value, B = t; break;
        case 3: R = p, G = q, B = value; break;
        case 4: R = t, G = p, B = value; break;
        case 5: R = value, G = p, B = q; break;
        default: return;
    }
}

