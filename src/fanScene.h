#ifndef FANSCENE_H

#define FANSCENE_H

#include <fanVector3.h>
#include <vector>

namespace fan {

class fanScene
{
public:
    std::vector<fanVector3<float> > mVertices;
};

}

#endif /* end of include guard: FANSCENE_H */
