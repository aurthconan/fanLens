#ifndef BRESENHAM_H

#define BRESENHAM_H

#include <algo/rasterize/fanLineGenerator.h>

class Bresenham
    : public fan::fanLineGenerator
{
public:
    virtual void plotLine( const fan::fanVector<float, 2>& p1,
                           const fan::fanVector<float, 2>& p2,
                           const fan::fanPixel& pixel,
                           fan::fanTexture<int, fan::fanPixel, 2>& film,
                           fan::fanLineGeneratorCallback* callback );
};

#endif /* end of include guard: BRESENHAM_H */
