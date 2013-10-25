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

fanPixel::fanPixel( const fanPixel& pixel )
    : a( pixel.a )
    , r( pixel.r )
    , g( pixel.g )
    , b( pixel.b )
{
}

fanPixel& fanPixel::operator=( const fanPixel& other ) {
    a = other.a;
    r = other.r;
    g = other.g;
    b = other.b;
    return *this;
}

fanPixel& fanPixel::operator+=( const fanPixel& other ) {
    if ( (255-other.a) > a ) {
        a += other.a;
    } else {
        a = 255;
    }
    if ( 255-other.r > r ) {
        r += other.r;
    } else {
        r = 255;
    }
    if ( 255-other.g > g ) {
        g += other.g;
    } else {
        g = 255;
    }
    if ( 255-other.b > b ) {
        b += other.b;
    } else {
        b = 255;
    }
    return *this;
}

fanPixel& fanPixel::operator*=( float ratio ) {
    if ( ratio < 0 || ratio > 1 ) {
        return *this;
    }
    // a *= ratio;
    r *= ratio;
    g *= ratio;
    b *= ratio;
    return *this;
}

fanPixel fanPixel::operator*( float ratio ) const {
    fanPixel result = *this;
    result *= ratio;
    return result;
}

fanPixel fanPixel::operator+( const fanPixel& other ) const {
    fanPixel result = *this;
    result += other;
    return result;
}

bool fanPixel::operator==( const fanPixel& other ) {
    if ( other.a == a && other.r == r && other.g == g && other.b == b ) {
        return true;
    }
    return false;
}

}

