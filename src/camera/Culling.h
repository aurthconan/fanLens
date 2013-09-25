#ifndef CULLING_H

#define CULLING_H

#include <fanLens.h>

inline bool Culling( const fan::fanVector3<float>& ori,
                     const fan::fanVector3<float>& center,
                     const fan::fanVector3<float>& normal ) {
    fan::fanVector3<float> view = center - ori;
    if ( view*normal <= 0 ) {
        return true;
    }
    return false;
}

#endif /* end of include guard: CULLING_H */
