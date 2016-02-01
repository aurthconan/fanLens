#ifndef FLATTEXTURE_H

#define FLATTEXTURE_H

#include <fanMaterial.h>
#include <boost/shared_ptr.hpp>

#include <fanTexture.h>
#include <fanPixel.h>

namespace fan {

class FlatTexture
    : public fanMaterial<float, 2>
{
public:
    FlatTexture( fanTexture<int, fanPixel, 2>* texture );

    virtual fanPixel getPixel( fanVector<float, 2>& dimens,
                               fanPixel incomming );

private:
    boost::shared_ptr<fanTexture<int, fanPixel, 2> > mTexture;
};

} /* namespace  */

#endif /* end of include guard: FLATTEXTURE_H */
