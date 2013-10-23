#ifndef FANSCENE_H

#define FANSCENE_H

#include <fanVector3.h>
#include <fanTriangle.h>
#include <fanLight.h>
#include <vector>

#include <fanTriangleMesh.h>
#include <boost/shared_ptr.hpp>

namespace fan {

class fanScene
{
public:
    std::vector<boost::shared_ptr<fanTriangleMesh> > mTriangleMeshes;
    std::vector<boost::shared_ptr<fanLight> > mLights;
};

}

#endif /* end of include guard: FANSCENE_H */
