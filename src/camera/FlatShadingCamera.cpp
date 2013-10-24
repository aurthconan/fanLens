#include "FlatShadingCamera.h"

#include <algo/rasterize/fanScanLineGenerator.h>
#include <lights/fanLightsAccumulator.h>
#include <fanScene.h>
#include <fanFilm.h>
#include <fanLens.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>

using namespace fan;

void FlatShadingCamera::begin( fan::fanScene& scene,
                               fan::fanFilm& film,
                               fan::fanLens& lens ) {
    mpLightsAccum.reset( new fanLightsAccumulator( scene.mLights ) );
    mpZBuffer.reset( new MemoryTexture<int, float, 2>( film.getDimens() ) );
    mpZBuffer->reset( 2.0f );
    mLensPos = lens.mPos;
}

void FlatShadingCamera::nextTriangle( fan::fanTriangleMesh& object,
                                      fan::fanTriangle& triangle ) {
    mPixel = mpLightsAccum->getLight( transform( object.mObjectToWorld,
                                                 triangle.mCenter ),
                                      triangle.mNormal, mLensPos );
}

void FlatShadingCamera::getCompaionData( size_t i,
                                         fanTriangle& triangle,
                                         fanVector<float,4>& coord,
                                         fanTriangleMesh& object,
                                         Data& data ) {
    (void) i; (void) triangle; (void) object;
    data.depth = coord[2];
}

void FlatShadingCamera::plot( fan::fanVector<float, 2> pos,
                              Data& data,
                              fan::fanFilm& film ) {
    if ( data.depth < mpZBuffer->getValue( pos ) ) {
        mpZBuffer->setValue( pos, data.depth );
        film.setValue( pos, mPixel );
    }
}

void FlatShadingCamera::end() {
}

FlatShadingCamera::Data::Data()
    : depth( 0 ) {
}

FlatShadingCamera::Data::Data( float _depth )
    : depth( _depth ) {
}


FlatShadingCamera::Data FlatShadingCamera::Data::operator-(
                            const Data& o ) const {
    FlatShadingCamera::Data result = *this;
    result.depth -= o.depth;
    return result;
}

FlatShadingCamera::Data FlatShadingCamera::Data::operator*(
                            const int& ratio ) const {
    FlatShadingCamera::Data result = *this;
    result.depth *= ratio;
    return result;
}

FlatShadingCamera::Data& FlatShadingCamera::Data::operator+=(
                            const FlatShadingCamera::Data& o ) {
    this->depth += o.depth;
    return *this;
}

FlatShadingCamera::Data FlatShadingCamera::Data::operator/(
                            const int& ratio ) const {
    FlatShadingCamera::Data result = *this;
    result.depth /= ratio;
    return result;
}

