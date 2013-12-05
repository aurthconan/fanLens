#ifndef RAINBOWLIGHT_H

#define RAINBOWLIGHT_H

#include <lights/PointLight.h>

class RainbowLight
    : public PointLight
{
public:
    RainbowLight( fan::fanVector3<float> pos );
    fan::fanPixel getLight( fan::fanVector3<float> world,
                            fan::fanVector3<float> normal,
                            fan::fanVector3<float> viewer );
};

#endif /* end of include guard: RAINBOWLIGHT_H */
