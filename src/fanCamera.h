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

    void takePicture( fanScene& scene,
                      fanFilm& film,
                      fanLens& lens );
};

}

#endif /* end of include guard: FANCAMERA_H */
