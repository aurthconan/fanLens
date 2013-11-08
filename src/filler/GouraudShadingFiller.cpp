#include "GouraudShadingFiller.h"

#include <lights/fanLightsAccumulator.h>
#include <fanScene.h>
#include <fanFilm.h>
#include <fanLens.h>
#include <fanTriangleMesh.h>
#include <fanTriangle.h>
#include <fanVector3.h>
#include <fanPixel.h>

void GouraudShadingFiller::begin( fan::fanScene& scene,
                                  fan::fanTexture<int, fan::fanPixel, 2>& film,
                                  fan::fanLens& lens ) {
    (void) film;
    mpLightsAccum.reset( new fan::fanLightsAccumulator( scene.mLights ) );
    mLensPos = lens.mPos;
}

void GouraudShadingFiller::end() {
}

void GouraudShadingFiller::nextTriangle( TriangleMeshObject& object,
                                         fan::fanTriangleMesh& mesh,
                                         fan::fanTriangle& triangle) {
    (void) object; (void) mesh; (void) triangle;
}

void GouraudShadingFiller::getCompaionData( size_t i,
                                            fan::fanTriangle& triangle,
                                            fan::fanTriangleMesh& mesh,
                                            TriangleMeshObject& object,
                                            fan::fanVector<float,4>& coord,
                                            Data& data ) {
    (void) coord;
    fan::fanVector3<float> normal = mesh.mNormals->mBuffer[triangle.pointsIndex[i]];
    fan::fanPixel pixel = mpLightsAccum->getLight( transform( object.mObjectToWorld,
                                                 *triangle.points[i] ),
                                      normal,
                                      mLensPos );
    data[0] = pixel.r;
    data[1] = pixel.g;
    data[2] = pixel.b;
}

void GouraudShadingFiller::plot( fan::fanVector<float, 2> pos,
                                 Data& data,
                                 float depth,
                                 fan::fanTexture<int, fan::fanPixel, 2>& film ) {
    (void) depth;
    fan::fanPixel pixel;
    pixel.a = 255; pixel.r = data[0];
    pixel.g = data[1]; pixel.b = data[2];
    film.setValue( pos, pixel );
}

