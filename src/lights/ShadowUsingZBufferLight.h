#ifndef SHADOWUSINGZBUFFERLIGHT_H

#define SHADOWUSINGZBUFFERLIGHT_H

#include <fanLight.h>
#include <fanCamera.h>
#include <lights/PointLight.h>
#include <texture/MemoryTexture.h>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <fanScene.h>
#include <lens/PerspectiveLens.h>

class ShadowUsingZBufferLight
    : public fan::fanLight
{
public:
    ShadowUsingZBufferLight( boost::shared_ptr<PointLight> point,
                             const fan::fanVector<float, 3>& dimens,
                             const fan::fanVector<float, 3>& pointAt );

    void bakeShadowMap( fan::fanScene& scene );

    fan::fanPixel getLight( fan::fanVector3<float> world,
                            fan::fanVector3<float> normal,
                            fan::fanVector3<float> viewer );

private:
    boost::shared_ptr<PointLight> mpPointLight;
    fan::fanVector<int, 2> mDimens;
    fan::fanVector<float, 3> m3Dimens;
    fan::fanVector<float, 3> mPointAt;
    // cube mapping of zbuffer
    boost::scoped_ptr<MemoryTexture<int, float, 2> > zNegative;

    boost::scoped_ptr<PerspectiveLens> zNLens;

};

#endif /* end of include guard: SHADOWUSINGZBUFFERLIGHT_H */
