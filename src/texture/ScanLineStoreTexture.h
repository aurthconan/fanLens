#ifndef SCANLINESTOREFILM_H

#define SCANLINESTOREFILM_H

#include <fanTexture.h>

// a class use to store and generate scanline
class ScanLineStoreTexture
    : public fan::fanTexture<int, float, 2>
{
public:
    ScanLineStoreTexture( const fan::fanVector<int, 2>& dimensions );
    ~ScanLineStoreTexture();

    void setValue( const fan::fanVector<int, 2>& index,
                   const float& value );

    void reset();

    int mYMin;
    int mYMax;
    fan::fanVector<int, 2>* mpYBucket;
    fan::fanVector<float, 2>* mpYValues;
};

#endif /* end of include guard: SCANLINESTOREFILM_H */
