#ifndef PERSPECTIVELENS_H

#define PERSPECTIVELENS_H

#include <fanLens.h>

class PerspectiveLens
    : public fan::fanLens
{
public:
    PerspectiveLens( fan::fanVector3<float> pos,
                     fan::fanVector3<float> lookAt,
                     fan::fanVector3<float> up,
                     fan::fanVector3<float> dimens,
                     float distance );

    bool cullFace( const fan::fanTriangle& triangle,
                   const fan::fanMatrix<float, 4, 4>& worldTransformation );


protected:
    virtual fan::fanMatrix<float, 4, 4> getTransformation();

    float mDistance;
};

#endif /* end of include guard: PERSPECTIVELENS_H */
