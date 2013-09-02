#ifndef ORTHOGONALLENS_H

#define ORTHOGONALLENS_H

#include <fanLens.h>

class OrthogonalLens
    : public fan::fanLens
{
public:
    OrthogonalLens( fan::fanVector3<float> pos,
                    fan::fanVector3<float> lookAt,
                    fan::fanVector3<float> up );

protected:
    bool projectToCameraSpace( fan::fanVector<int, 2>& pos,
                               const fan::fanVector3<float>& world );
};

#endif /* end of include guard: ORTHOGONALLENS_H */
