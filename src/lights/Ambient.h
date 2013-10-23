#ifndef AMBIENT_H

#define AMBIENT_H

#include <fanLight.h>
#include <fanPixel.h>

class Ambient
    : public fan::fanLight
{
public:
    Ambient( fan::fanPixel pixel );
    virtual fan::fanPixel getLight( fan::fanVector3<float> world,
                                    fan::fanVector3<float> normal,
                                    fan::fanVector3<float> viewer );
private:
    fan::fanPixel mPixel;
};

#endif /* end of include guard: AMBIENT_H */

