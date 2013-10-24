#ifndef FLATSHADINGCAMERA_H

#define FLATSHADINGCAMERA_H

#include <camera/RasterisationCamera.h>
#include <fanPixel.h>
#include <lights/fanLightsAccumulator.h>
#include <texture/MemoryTexture.h>
#include <boost/scoped_ptr.hpp>

class FlatShadingCompaionData {
public:
    FlatShadingCompaionData();
    FlatShadingCompaionData( float depth );

    FlatShadingCompaionData operator-( const FlatShadingCompaionData& o ) const;
    FlatShadingCompaionData operator*( const int& ratio ) const;
    FlatShadingCompaionData& operator+=( const FlatShadingCompaionData& o );
    FlatShadingCompaionData operator/( const int& ratio ) const;

    float depth;
};

class FlatShadingCamera
    : public RasterisationCamera<FlatShadingCompaionData>
{
public:
    virtual void begin( fan::fanScene& scene,
                        fan::fanFilm& film,
                        fan::fanLens& lens );
    virtual void end();
    virtual void nextTriangle( fan::fanTriangleMesh& object,
                               fan::fanTriangle& triangle );
    virtual void getCompaionData( size_t i,
                                  fan::fanTriangle& triangle,
                                  fan::fanVector<float,4>& coord,
                                  fan::fanTriangleMesh& object,
                                  FlatShadingCompaionData& data );
    virtual void plot( fan::fanVector<float, 2> pos,
                       FlatShadingCompaionData& data,
                       fan::fanFilm& film );
private:
    boost::scoped_ptr<fan::fanLightsAccumulator> mpLightsAccum;
    boost::scoped_ptr<MemoryTexture<int, float, 2> > mpZBuffer;
    fan::fanPixel mPixel;
    fan::fanVector3<float> mLensPos;
};

#endif /* end of include guard: FLATSHADINGCAMERA_H */
