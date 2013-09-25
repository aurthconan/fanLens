#ifndef ORTHOGONALLENS_H

#define ORTHOGONALLENS_H

#include <fanLens.h>

class OrthogonalLens
    : public fan::fanLens
{
public:
    OrthogonalLens( fan::fanVector3<float> pos,
                    fan::fanVector3<float> lookAt,
                    fan::fanVector3<float> up,
                    fan::fanVector3<float> dimens );

protected:
    virtual fan::fanMatrix<float, 4, 4> getTransformation();

};

#endif /* end of include guard: ORTHOGONALLENS_H */
