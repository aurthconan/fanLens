#ifndef PHONGSHADINGFILLER_H

#define PHONGSHADINGFILLER_H

#include <fanVector3.h>
#include <lights/fanLightsAccumulator.h>
#include <objects/TriangleMeshObject.h>
#include <boost/scoped_ptr.hpp>

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

        Data& operator*=( const int& ratio ) {
            this->pos *= ratio;
            this->normal *= ratio;
            return *this;
        }

        Data& operator/=( const int& ratio ) {
            this->pos /= ratio;
            this->normal /= ratio;
            return *this;
        }

        fan::fanVector3<float> pos;
        fan::fanVector3<float> normal;
    };

    void begin( fan::fanScene& scene,
                fan::fanFilm& film,
                fan::fanLens& lens );
    void end();
    void nextTriangle( TriangleMeshObject& object,
                       fan::fanTriangleMesh& mesh,
                       fan::fanTriangle& triangle );
    void getCompaionData( size_t i,
                          fan::fanTriangle& triangle,
                          fan::fanTriangleMesh& mesh,
                          TriangleMeshObject& object,
                          fan::fanVector<float,4>& coord,
                          Data& data );
    void plot( fan::fanVector<float, 2> pos,
               Data& data,
               float depth,
               fan::fanFilm& film );

private:
    boost::scoped_ptr<fan::fanLightsAccumulator> mpLightsAccum;
    fan::fanVector3<float> mLensPos;
};

#endif /* end of include guard: PHONGSHADINGFILLER_H */
