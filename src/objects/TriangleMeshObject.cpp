#include "TriangleMeshObject.h"

TriangleMeshObject::TriangleMeshObject( const fan::fanMatrix<float, 4, 4>& objectToWorld )
    : fanObject( objectToWorld ) {
}

