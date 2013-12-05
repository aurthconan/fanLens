#ifndef RGBCONVERTER_H

#define RGBCONVERTER_H

class RGBConverter
{
public:
   static void fromHSVToRGB( const float hue_angle,
                             const float saturation,
                             const float value,
                             float& R,
                             float& G,
                             float& B );
};

#endif /* end of include guard: RGBCONVERTER_H */
