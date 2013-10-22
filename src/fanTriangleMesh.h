#ifndef FANTRIANGLEMESH_H

#define FANTRIANGLEMESH_H

#include <cstddef>
#include <fanVector3.h>
#include <fanObject.h>
#include <fanTriangle.h>
#include <fanBufferObject.h>

#include <boost/shared_ptr.hpp>
#include <vector>

namespace fan {

class fanTriangleMesh
    : public fan::fanObject
{
public:
    fanTriangleMesh( const fan::fanMatrix<float, 4, 4>& objectToWorld );
    virtual ~fanTriangleMesh();

    std::vector< boost::shared_ptr< fanBufferObject<fanVector3<float> > > > mVertices;
    std::vector< boost::shared_ptr< fanBufferObject<fanTriangle> > > mFaces;
};

} /* namespace fan */

#endif /* end of include guard: FANTRIANGLEMESH_H */
