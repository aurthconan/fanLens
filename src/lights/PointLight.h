#ifndef POINTLIGHT_H

#define POINTLIGHT_H

#include <fanLight.h>
#include <fanPixel.h>

// point light without attenuation
class PointLight
    : public fan::fanLight
{
public:
    PointLight( fan::fanVector3<float> pos,
                fan::fanPixel light );
    fan::fanPixel getLight( fan::fanVector3<float> world,
                            fan::fanVector3<float> normal,
                            fan::fanVector3<float> viewer );

    fan::fanVector3<float> mPos;
    fan::fanPixel mLightPixel;
};

#endif /* end of include guard: POINTLIGHT_H */
