#ifndef FANOBJECT_H

#define FANOBJECT_H

#include <fanVector.h>
#include <fanMatrix.h>

namespace fan {

// represent a model in the scene
class fanObject
{
public:
    fanObject( const fan::fanMatrix<float, 4, 4>& objectToWorld );
    virtual ~fanObject();

    fan::fanMatrix<float, 4, 4> mObjectToWorld;
};

} /* namespace fan */

#endif /* end of include guard: FANOBJECT_H */
