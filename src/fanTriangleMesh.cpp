#include "fanTriangleMesh.h"

namespace fan {


fanTriangleMesh::fanTriangleMesh(
        const fan::fanMatrix<float, 4, 4>& objectToWorld )
    : fanObject( objectToWorld ) {
}

fanTriangleMesh::~fanTriangleMesh() {
}

} /* namespace fan */

