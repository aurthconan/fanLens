#include "fanPixel.h"

namespace fan {

fanPixel::fanPixel( unsigned short _a, unsigned short _r,
                    unsigned short _g, unsigned short _b )
    : a( _a )
    , r( _r )
    , g( _g )
    , b( _b )
{
}

fanPixel::fanPixel( int color )
    : c( color )
{
}

}

