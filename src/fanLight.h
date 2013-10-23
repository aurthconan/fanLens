#ifndef FANLIGHT_H

#define FANLIGHT_H

#include <fanVector3.h>

namespace fan {

class fanPixel;

class fanLight
{
public:
    virtual ~fanLight();
    virtual fan::fanPixel getLight( fan::fanVector3<float> world,
                                    fan::fanVector3<float> normal,
                                    fan::fanVector3<float> viewer ) = 0;
};

} /* namespace fan */

#endif /* end oe guard: FANLIGHT_H */
