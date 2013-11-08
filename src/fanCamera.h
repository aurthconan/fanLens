#ifndef FANCAMERA_H

#define FANCAMERA_H

#include <fanScene.h>
#include <fanLens.h>
#include <fanFilm.h>
#include <fanScanner.h>

namespace fan {

/* class for trigger render */
class fanCamera
    : public fan::fanScanner<int, fanPixel, 2>
{
public:
    virtual ~fanCamera();

    virtual void takePicture( fan::fanScene& scene,
                              fan::fanTexture<int, fan::fanPixel, 2>& film,
                              fan::fanLens& lens ) = 0;
};

}

#endif /* end of include guard: FANCAMERA_H */
