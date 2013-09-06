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
                     float distance );

protected:
    bool projectInCameraSpace( fan::fanVector<float, 2>& pos,
                               const fan::fanVector3<float>& camera );
    virtual fan::fanMatrix<float, 4, 4> getTransformation();

    float mDistance;
};

#endif /* end of include guard: PERSPECTIVELENS_H */
