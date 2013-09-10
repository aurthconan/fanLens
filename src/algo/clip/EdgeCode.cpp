#include "EdgeCode.h"

unsigned short EncodeDot( const fan::fanVector<float, 2>& dot,
                          const fan::fanVector<float, 4>& window ) {
    unsigned short code = 0;
    if ( dot[0] < window[LEFT_BIT] ) {
        code |= LEFT_MASK;
    } else if ( dot[0] > window[RIGHT_BIT] ) {
        code |= RIGHT_MASK;
    }
    if ( dot[1] < window[BELOW_BIT] ) {
        code |= BELOW_MASK;
    } else if ( dot[1] > window[ABOVE_BIT] ) {
        code |= ABOVE_MASK;
    }
    return code;
}

