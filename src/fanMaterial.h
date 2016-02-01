#ifndef FANMATERIAL_H

#define FANMATERIAL_H

#include "fanPixel.h"
#include "fanVector.h"

namespace fan {

template<typename T, size_t DIMENS>
class fanMaterial
{
public:
    virtual ~fanMaterial(){
    }

    virtual fanPixel getPixel( fanVector<T, DIMENS>& dimens,
                               fanPixel incomming ) = 0;
};

} /* namespace fanLens */


#endif /* end of include guard: FANMATERIAL_H */
