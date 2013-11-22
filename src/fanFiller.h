#ifndef FANFILLER_H

#define FANFILLER_H

namespace fan {
class fanScene;
class fanLens;
class fanTriangleMesh;
class fanTriangle;
template<typename, typename, size_t>
class fanTexture;
} /* namespace fan */

class TriangleMeshObject;

namespace fan {

class fanFiller
{
public:
    typedef int Data;

    inline void begin( fan::fanScene& scene,
                       fan::fanTexture<int, float, 2>& texture,
                       fan::fanLens& lens )
    { (void) scene; (void) texture; (void) lens; };

    inline void end() {};

    inline void nextTriangle( TriangleMeshObject& object,
                              fan::fanTriangleMesh& mesh,
                              fan::fanTriangle& triangle )
    { (void) object; (void) mesh; (void) triangle; };

    inline void getCompaionData( size_t i,
                                 fan::fanTriangle& triangle,
                                 fan::fanTriangleMesh& mesh,
                                 TriangleMeshObject& object,
                                 fan::fanVector<float,4>& coord,
                                 Data& data )
    { (void) i; (void) triangle; (void) mesh;
      (void) object; (void) coord; (void) data; };

    inline void plot( fan::fanVector<float, 2> pos,
                      Data& data,
                      float depth,
                      fan::fanTexture<int, float, 2>& texture )
    { (void) data; (void) pos; (void) depth; (void) texture; }

};

} /* namespace fan */

#endif /* end of include guard: FANFILLER_H */
