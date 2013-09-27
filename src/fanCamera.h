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

    inline bool project( const fan::fanVector3<float>& world,
                         const fan::fanLens& lens,
                         const fan::fanVector<int, 2>& screen,
                         fan::fanVector<float, 2>& pos,
                         fan::fanVector<float, 4>& homoPos ) {
        bool visible = true;
        homoPos = world - lens.mPos;
        homoPos[3] = 1.0f;
        homoPos = lens.mViewTransformation * homoPos;
        homoPos[0] /= homoPos[3];
        homoPos[1] /= homoPos[3];

        if ( homoPos[0] < 0 || homoPos[0] > 1
                || homoPos[1] < 0 || homoPos[1] > 1
                || homoPos[2] < 0 || homoPos[2] > 1 ) {
            visible = false;
        }

        pos[0] = homoPos[0]*screen[0];
        pos[1] = homoPos[1]*screen[1];

        return visible;
    }
};

}

#endif /* end of include guard: FANCAMERA_H */
