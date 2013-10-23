#ifndef FANPIXEL_H

#define FANPIXEL_H

#include <fanVector.h>

namespace fan {

class fanPixel
{
public:
    union {
        struct {
        unsigned char a, r, g, b;
        };
    };

    fanPixel( unsigned char a, unsigned char r,
              unsigned char g, unsigned char b );
    fanPixel( int color = 0 );
    fanPixel( const fanPixel& pixel );

    fanPixel& operator=( const fanPixel& other );
    fanPixel& operator+=( const fanPixel& other );
    fanPixel& operator*=( float ratio );
    fanPixel operator*( float ratio );

    bool operator==( const fanPixel& other );
};

}

#endif /* end of include guard: FANPIXEL_H */
