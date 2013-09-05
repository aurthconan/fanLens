#ifndef WIREFRAMECAMERA_H

#define WIREFRAMECAMERA_H

#include <fanCamera.h>

class WireframeCamera
    : public fan::fanCamera
{
public:
    virtual void takePicture( fan::fanScene& scene,
                              fan::fanFilm& film,
                              fan::fanLens& lens );
};

#endif /* end of include guard: WIREFRAMECAMERA_H */
