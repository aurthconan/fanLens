#ifndef SCANLINESTOREFILM_H

#define SCANLINESTOREFILM_H

#include <fanFilm.h>

// a class use to store and generate scanline
class ScanLineStoreFilm
    : public fan::fanFilm
{
public:
    ScanLineStoreFilm( const fan::fanVector<int, 2>& dimensions );
    ~ScanLineStoreFilm();

    void setValue( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel );

    void reset();

    int mYMin;
    int mYMax;
    fan::fanVector<int, 2>* mpYBucket;
};

#endif /* end of include guard: SCANLINESTOREFILM_H */
