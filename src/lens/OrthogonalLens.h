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

    fan::fanVector<int, 2> project( const fan::fanVector3<float>& world,
                                    const fan::fanTexture<int, 2>& space );
};

#endif /* end of include guard: ORTHOGONALLENS_H */
