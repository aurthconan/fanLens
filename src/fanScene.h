#ifndef FANSCENE_H

#define FANSCENE_H

#include <fanVector3.h>
#include <fanTriangle.h>
#include <vector>

#include <fanTriangleMesh.h>

namespace fan {

class fanScene
{
public:
    std::vector<fanTriangleMesh> mTriangleMeshes;
};

}

#endif /* end of include guard: FANSCENE_H */
