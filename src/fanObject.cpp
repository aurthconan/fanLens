#include <fanObject.h>

namespace fan {

fanObject::fanObject( const fan::fanMatrix<float, 4, 4>& objectToWorld )
    : mObjectToWorld( objectToWorld ) {
}

fanObject::~fanObject() {
}

} /* namespace fan */
