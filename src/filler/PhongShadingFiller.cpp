#include "PhongShadingFiller.h"

#include <lights/fanLightsAccumulator.h>
#include <fanScene.h>
#include <fanFilm.h>
#include <fanLens.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>
#include <fanVector3.h>
#include <fanPixel.h>

void PhongShadingFiller::begin( fan::fanScene& scene,
                                fan::fanFilm& film,
                                fan::fanLens& lens ) {
    (void) film;
    mpLightsAccum.reset( new fan::fanLightsAccumulator( scene.mLights ) );
    mLensPos = lens.mPos;
}

void PhongShadingFiller::end() {
}

void PhongShadingFiller::nextTriangle( TriangleMeshObject& object,
                                       fan::fanTriangleMesh& mesh,
                                       fan::fanTriangle& triangle ) {
}

void PhongShadingFiller::getCompaionData( size_t i,
                                          fan::fanTriangle& triangle,
                                          fan::fanTriangleMesh& mesh,
                                          TriangleMeshObject& object,
                                          fan::fanVector<float,4>& coord,
                                          Data& data ) {
    data.normal = mesh.mNormals->mBuffer[triangle.pointsIndex[i]];
    data.pos = transform( object.mObjectToWorld, *triangle.points[i] );
}

void PhongShadingFiller::plot( fan::fanVector<float, 2> pos,
                               Data& data,
                               float depth,
                               fan::fanFilm& film ) {
    fan::fanPixel pixel = mpLightsAccum->getLight( data.pos,
                                      normalize( data.normal ),
                                      mLensPos );
    film.setValue( pos, pixel );
}

