#ifndef FANSCANNER_H

#define FANSCANNER_H

#include <fanTexture.h>

namespace fan {

inline bool project( const fan::fanVector3<float>& world,
                     const fanLens& lens,
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

// generalize class for cast scene to some texture
template<typename DIMENS_TYPE, typename VALUE_TYPE, size_t DIMENS>
class fanScanner
{
public:
    virtual ~fanScanner() {};

    virtual void takePicture( fan::fanScene& scene,
                              fan::fanTexture<DIMENS_TYPE, VALUE_TYPE, DIMENS>& dest,
                              fan::fanLens& lens ) = 0;

};

} /* namespace fan */

#endif /* end of include guard: FANSCANNER_H */
