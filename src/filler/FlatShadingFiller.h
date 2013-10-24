#ifndef FLATSHADINGCAMERA_H

#define FLATSHADINGCAMERA_H

#include <fanPixel.h>
#include <lights/fanLightsAccumulator.h>
#include <texture/MemoryTexture.h>
#include <boost/scoped_ptr.hpp>

namespace fan {
class fanScene;
class fanFilm;
class fanLens;
class fanTriangleMesh;
class fanTriangle;
} /* namespace fan */

class FlatShadingFiller
{
public:
    typedef int Data;

    void begin( fan::fanScene& scene,
                fan::fanFilm& film,
                fan::fanLens& lens );
    void end();
    void nextTriangle( fan::fanTriangleMesh& object,
                       fan::fanTriangle& triangle );
    void getCompaionData( size_t i,
                          fan::fanTriangle& triangle,
                          fan::fanVector<float,4>& coord,
                          fan::fanTriangleMesh& object,
                          Data& data );
    void plot( fan::fanVector<float, 2> pos,
               Data& data,
               float depth,
               fan::fanFilm& film );

private:
    boost::scoped_ptr<fan::fanLightsAccumulator> mpLightsAccum;
    boost::scoped_ptr<MemoryTexture<int, float, 2> > mpZBuffer;
    fan::fanPixel mPixel;
    fan::fanVector3<float> mLensPos;
};

#endif /* end of include guard: FLATSHADINGCAMERA_H */
