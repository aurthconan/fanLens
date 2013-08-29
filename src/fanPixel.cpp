#include "fanPixel.h"

namespace fan {

fanPixel::fanPixel( unsigned char _a, unsigned char _r,
                    unsigned char _g, unsigned char _b )
    : a( _a )
    , r( _r )
    , g( _g )
    , b( _b )
{
}

fanPixel::fanPixel( int color )
    : a( color & 0xff000000 )
    , r( color & 0x00ff0000 )
    , g( color & 0x0000ff00 )
    , b( color & 0x000000ff )
{
}

}

