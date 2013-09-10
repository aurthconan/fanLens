#include "CohenSutherland.h"

#include <algo/clip/EdgeCode.h>

#include <algorithm>

using namespace fan;

namespace {

inline unsigned short Sum( unsigned short code ) {
    unsigned short sum = 0;
    for ( size_t i = 0; i < 4; ++i ) {
        sum += (code & (1 << i)) >> i;
    }
    return sum;
}

enum VisibleFlag {
    TRIVIALLY_VISIBLE,
    PARTIAL_VISIBLE,
    TRIVIALLY_INVISIBLE,
};

inline VisibleFlag Visible( unsigned short p1code,
                            unsigned short p2code ) {
    if ( p1code == 0 && p2code == 0 )
        return TRIVIALLY_VISIBLE;

    if ( (p1code & p2code) != 0 )
        return TRIVIALLY_INVISIBLE;

    return PARTIAL_VISIBLE;
}

}

bool CohenSutherland( const fanVector<float, 4>& window,
                      fanVector<float, 2>& p1,
                      fanVector<float, 2>& p2 ) {
    bool swaped = false;
    unsigned short p1code = EncodeDot( p1, window );
    unsigned short p2code = EncodeDot( p2, window );
    //unsigned short p1sum = Sum( p1code );
    //unsigned short p2sum = Sum( p2code );
    VisibleFlag vFlag = Visible( p1code, p2code );
    if ( vFlag == TRIVIALLY_VISIBLE ) {
        return true;
    }
    if ( vFlag == TRIVIALLY_INVISIBLE ) {
        return false;
    }

    enum {
        VERTICAL,
        HORIZONTAL,
    };
    short sFlag = -1; // special case line
    float slope = 0;
    if ( p1[0] == p2[0] ) {
        sFlag = VERTICAL;
    } else if ( p1[1] == p2[1] ) {
        sFlag = HORIZONTAL;
    } else {
        slope = ((p2[1]-p1[1])/(p2[0]-p1[0]));
    }

    for ( size_t i = 0; i < 4; ++i ) {
        short p1_c = p1code&(1<<(i)); short p2_c = p2code&(1<<(i));
        if ( (p1code&(1<<(i))) == (p2code&(1<<(i))) ) {
            continue;
        }

        if ( (p1code&(1<<(i))) == 0 ) {
            swaped = !swaped;
            std::swap(p1, p2);
            std::swap(p1code, p2code);
        }

        if ( i < 2 ) {
            if ( sFlag != HORIZONTAL ) {
                p1[1] = slope*(window[i]-p1[0]) + p1[1];
            }
            p1[0] = window[i];
        } else {
            if ( sFlag != VERTICAL ) {
                p1[0] = (1/slope)*(window[i]-p1[1]) + p1[0];
            }
            p1[1] = window[i];
        }

        p1code = EncodeDot( p1, window );
        VisibleFlag vFlag = Visible( p1code, p2code );
        if ( vFlag == TRIVIALLY_VISIBLE ) {
            if ( swaped ) {
                std::swap(p1, p2);
            }
            return true;
        }
        if ( vFlag == TRIVIALLY_INVISIBLE ) {
            return false;
        }
    }
}

