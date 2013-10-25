#ifndef FANTRIANGLEMESH_H

#define FANTRIANGLEMESH_H

#include <cstddef>
#include <fanVector3.h>
#include <fanTriangle.h>
#include <fanBufferObject.h>
#include <boost/shared_ptr.hpp>

namespace fan {

class fanTriangleMesh
{
public:
    boost::shared_ptr<fanBufferObject<fanVector3<float> > > mVertices;
    boost::shared_ptr<fanBufferObject<fanTriangle> > mFaces;
    boost::shared_ptr<fanBufferObject<fanVector3<float> > > mNormals;
};

} /* namespace fan */

#endif /* end of include guard: FANTRIANGLEMESH_H */
