#ifndef QUADTREE_H

#define QUADTREE_H

#include <boost/shared_ptr.hpp>

#include <fanVector3.h>
#include <fanTriangleMesh.h>
#include <objects/TriangleMeshObject.h>

class PointGenerator;

class QuadTree
{
public:
    // c - d
    // |   |
    // a - b
    QuadTree( const fan::fanVector3<float>& a,
              const fan::fanVector3<float>& b,
              const fan::fanVector3<float>& c,
              const fan::fanVector3<float>& d );

    QuadTree( size_t level, fan::fanVector<size_t, 2> index,
              PointGenerator* generator );
    ~QuadTree();

    void generateTerrian( TriangleMeshObject& terrian,
                          // fan::fanVector3<float>& eyePos );
                          size_t level );

private:
    QuadTree();

    const size_t mLevel;
    const fan::fanVector<size_t, 2> mIndex;

    PointGenerator* mPointGenerator;

    void initThisTile();
    boost::shared_ptr<fan::fanTriangleMesh> mThisTile;
    boost::shared_ptr<QuadTree> mChildTiles[2][2];
};

#endif /* end of include guard: QUADTREE_H */
