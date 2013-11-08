#ifndef DEPTHCAMERA_H

#define DEPTHCAMERA_H

#include <fanCamera.h>

class DepthCamera
    : public fan::fanCamera
{
public:
    virtual void takePicture( fan::fanScene& scene,
                              fan::fanTexture<int, fan::fanPixel, 2>& film,
                              fan::fanLens& lens );
};

#endif /* end of include guard: DEPTHCAMERA_H */
