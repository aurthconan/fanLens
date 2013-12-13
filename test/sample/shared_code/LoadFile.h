#ifndef LOADFILE_H

#define LOADFILE_H

#include <fanVector3.h>

namespace fan {
    class fanScene;
}

bool loadFile( char* file,
               fan::fanScene& outScene,
               fan::fanVector3<float>& boundBoxMin,
               fan::fanVector3<float>& boundBoxMax );

#endif /* end of include guard: LOADFILE_H */
