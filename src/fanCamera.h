#ifndef FANCAMERA_H

#define FANCAMERA_H

#include "fanScene.h"
#include "fanLens.h"
#include "fanFilm.h"

namespace fan {

/* class for trigger render */
class fanCamera
{
public:
    virtual ~fanCamera();

    virtual void takePicture( fan::fanScene& scene,
                              fan::fanFilm& film,
                              fan::fanLens& lens );
};

}

#endif /* end of include guard: FANCAMERA_H */
