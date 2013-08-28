#ifndef FANPIXEL_H

#define FANPIXEL_H

#include <fanVector.h>

namespace fan {

class fanPixel
{
public:
    union {
        unsigned short a, r, g, b;
        int c;
    };

    fanPixel( unsigned short a, unsigned short r,
              unsigned short g, unsigned short b );
    fanPixel( int color );
};

}

#endif /* end of include guard: FANPIXEL_H */
