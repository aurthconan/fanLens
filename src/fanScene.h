#ifndef FANSCENE_H

#define FANSCENE_H

#include <fanVector.h>
#include <vector>

namespace fan {

class fanScene
{
public:
    std::vector<fanVector<float, 3> > mVertices;
};

}

#endif /* end of include guard: FANSCENE_H */
