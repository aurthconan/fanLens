#include "GouraudShadingFiller.h"

#include <lights/fanLightsAccumulator.h>
#include <fanScene.h>
#include <fanFilm.h>
#include <fanLens.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>

void GouraudShadingFiller::begin( fan::fanScene& scene,
                                  fan::fanFilm& film,
                                  fan::fanLens& lens ) {
    mpLightsAccum.reset( new fan::fanLightsAccumulator( scene.mLights ) );
    mLensPos = lens.mPos;
}

void GouraudShadingFiller::end() {
}

void GouraudShadingFiller::nextTriangle( TriangleMeshObject& object,
                                         fan::fanTriangleMesh& mesh,
                                         fan::fanTriangle& triangle) {
}

void GouraudShadingFiller::getCompaionData( size_t i,
                                            fan::fanTriangle& triangle,
                                            fan::fanTriangleMesh& mesh,
                                            TriangleMeshObject& object,
                                            fan::fanVector<float,4>& coord,
                                            Data& data ) {
    /*
    data = mpLightsAccum->getLight( transform( object.mObjectToWorld,
                                                 triangle.points[i] ),
                                      triangle.mNormal, mLensPos );
                                      */
}

void GouraudShadingFiller::plot( fan::fanVector<float, 2> pos,
                                 Data& data,
                                 float depth,
                                 fan::fanFilm& film ) {
    (void) depth;
    film.setValue( pos, data );
}

