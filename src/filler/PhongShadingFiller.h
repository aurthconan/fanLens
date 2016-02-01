#ifndef PHONGSHADINGFILLER_H

#define PHONGSHADINGFILLER_H

#include <fanVector3.h>
#include <lights/fanLightsAccumulator.h>
#include <objects/TriangleMeshObject.h>
#include <boost/shared_ptr.hpp>

namespace fan {
class fanScene;
class fanFilm;
class fanLens;
class fanTriangleMesh;
class fanTriangle;
} /* namespace fan */

class PhongShadingFiller
{
public:
    class Data {
    public:
        Data& operator+=( const Data& o ) {
            this->pos += o.pos;
            this->normal += o.normal;
            return *this;
        }

        Data& operator-=( const Data& o ) {
            this->pos -= o.pos;
            this->normal -= o.normal;
            return *this;
        }

        Data& operator*=( const float& ratio ) {
            this->pos *= ratio;
            this->normal *= ratio;
            return *this;
        }

        Data& operator/=( const float& ratio ) {
            this->pos /= ratio;
            this->normal /= ratio;
            return *this;
        }

        fan::fanVector3<float> pos;
        fan::fanVector3<float> normal;
    };

    inline void begin( fan::fanScene& scene,
                fan::fanTexture<int, fan::fanPixel, 2>& film,
                fan::fanLens& lens )
    {
        (void) film;
        mpLightsAccum.reset( new fan::fanLightsAccumulator( scene.mLights ) );
        mLensPos = lens.mPos;
    }
    inline void end() {}
    inline void nextTriangle( TriangleMeshObject& object,
                       fan::fanTriangleMesh& mesh,
                       fan::fanTriangle& triangle )
    { (void) object; (void) mesh; (void) triangle; }
    inline void getCompaionData( size_t i,
                          fan::fanTriangle& triangle,
                          fan::fanTriangleMesh& mesh,
                          TriangleMeshObject& object,
                          fan::fanVector<float,4>& coord,
                          Data& data )
    {
        (void) i; (void) coord; (void) data;
        data.normal = mesh.mNormals->mBuffer[triangle.pointsIndex[i]];
        data.pos = transform( object.mObjectToWorld, *triangle.points[i] );
    }

    inline void plot( fan::fanVector<float, 2> pos,
               Data& data,
               float depth,
               fan::fanTexture<int, fan::fanPixel, 2>& film )
    {
        (void) depth;
        fan::fanPixel pixel = mpLightsAccum->getLight( data.pos,
                                          normalize( data.normal ),
                                          mLensPos );
        film.setValue( pos, pixel );
    }

private:
    boost::shared_ptr<fan::fanLightsAccumulator> mpLightsAccum;
    fan::fanVector3<float> mLensPos;
};

#endif /* end of include guard: PHONGSHADINGFILLER_H */
