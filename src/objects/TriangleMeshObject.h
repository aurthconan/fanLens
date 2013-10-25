#ifndef TRIANGLEMESHOBJECT_H

#define TRIANGLEMESHOBJECT_H

#include <fanObject.h>
#include <fanTriangleMesh.h>

#include <vector>
#include <boost/shared_ptr.hpp>

class TriangleMeshObject
    : public fan::fanObject
{
public:
    TriangleMeshObject( const fan::fanMatrix<float, 4, 4>& objectToWorld );

    std::vector< boost::shared_ptr< fan::fanTriangleMesh > > mMeshes;
};

#endif /* end of include guard: TRIANGLEMESHOBJECT_H */
