#include "FlatShadingFiller.h"

#include <lights/fanLightsAccumulator.h>
#include <fanScene.h>
#include <fanFilm.h>
#include <fanLens.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>

using namespace fan;

void FlatShadingFiller::begin( fan::fanScene& scene,
                               fan::fanFilm& film,
                               fan::fanLens& lens ) {
    (void) film;
    mpLightsAccum.reset( new fanLightsAccumulator( scene.mLights ) );
    mLensPos = lens.mPos;
}

void FlatShadingFiller::nextTriangle( TriangleMeshObject& object,
                                      fanTriangleMesh& mesh,
                                      fanTriangle& triangle ) {
    (void) mesh;
    mPixel = mpLightsAccum->getLight( transform( object.mObjectToWorld,
                                                 triangle.mCenter ),
                                      triangle.mNormal, mLensPos );
}

void FlatShadingFiller::getCompaionData( size_t i,
                                         fan::fanTriangle& triangle,
                                         fan::fanTriangleMesh& mesh,
                                         TriangleMeshObject& object,
                                         fan::fanVector<float,4>& coord,
                                         Data& data ) {
    (void) i; (void) triangle; (void) mesh; (void) object; (void) coord;
    data = 0;
}

void FlatShadingFiller::plot( fan::fanVector<float, 2> pos,
                              Data& data,
                              float depth,
                              fan::fanFilm& film ) {
    (void) depth; (void) data;
    film.setValue( pos, mPixel );
}

void FlatShadingFiller::end() {
}

