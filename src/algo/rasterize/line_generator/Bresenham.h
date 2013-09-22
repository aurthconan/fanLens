#ifndef BRESENHAM_H

#define BRESENHAM_H

#include <algo/rasterize/fanLineGenerator.h>

class Bresenham
{
public:
    virtual void plotLine( const fan::fanVector<float, 2>& p1,
                           const fan::fanVector<float, 2>& p2,
                           fan::fanFilm& film );
};

#endif /* end of include guard: BRESENHAM_H */
