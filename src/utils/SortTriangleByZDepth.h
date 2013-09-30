#ifndef SORTTRIANGLEBYZDEPTH_H

#define SORTTRIANGLEBYZDEPTH_H

namespace fan {
    class fanLens;
    class fanTriangle;
}

class SortTriangleByZDepth
{
public:
    SortTriangleByZDepth( fan::fanLens& lens );
    bool operator()( const fan::fanTriangle& l,
                     const fan::fanTriangle& r );

private:
    fan::fanLens& mLens;
};

#endif /* end of include guard: SORTTRIANGLEBYZDEPTH_H */
