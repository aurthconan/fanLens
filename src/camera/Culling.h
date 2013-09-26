#ifndef CULLING_H

#define CULLING_H

#include <fanLens.h>

inline bool Culling( const fan::fanLens& lens,
                     const fan::fanVector3<float>& normal ) {
    fan::fanVector3<float> view = lens.mLookAt - lens.mPos;
    if ( view*normal <= 0 ) {
        return true;
    }
    return false;
}

#endif /* end of include guard: CULLING_H */
