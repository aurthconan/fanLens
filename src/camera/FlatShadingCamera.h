#ifndef FLATSHADINGCAMERA_H

#define FLATSHADINGCAMERA_H

#include <fanCamera.h>

class FlatShadingCamera
    : public fan::fanCamera
{
public:
    virtual void takePicture( fan::fanScene& scene,
                              fan::fanFilm& film,
                              fan::fanLens& lens );
};

#endif /* end of include guard: FLATSHADINGCAMERA_H */
