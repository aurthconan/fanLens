#ifndef POINTSCANNERCAMERA_H

#define POINTSCANNERCAMERA_H

#include <fanCamera.h>

class PointScannerCamera
    : public fan::fanCamera
{
public:
    virtual void takePicture( fan::fanScene& scene,
                              fan::fanTexture<int, fan::fanPixel, 2>& film,
                              fan::fanLens& lens );
};

#endif /* end of include guard: POINTSCANNERCAMERA_H */
