#ifndef FANFILM_H

#define FANFILM_H

#include "fanTexture.h"

namespace fan {

/* A 2d space film for record image */
class fanFilm
    : public fanTexture<int, 2>
{
public:
    fanFilm( const fanVector<int, 2>& dimensions );
    virtual ~fanFilm();
};

}

#endif /* end of include guard: FANFILM_H */
