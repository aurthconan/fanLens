#ifndef FANSCENE_H

#define FANSCENE_H

#include <fanVector3.h>
#include <fanTriangle.h>
#include <fanLight.h>
#include <objects/TriangleMeshObject.h>

#include <vector>
#include <boost/shared_ptr.hpp>

namespace fan {

class fanScene
{
public:
    std::vector<boost::shared_ptr<TriangleMeshObject> > mTriangleMeshObjects;
    std::vector<boost::shared_ptr<fanLight> > mLights;
};

}

#endif /* end of include guard: FANSCENE_H */
