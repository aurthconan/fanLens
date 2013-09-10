#ifndef EDGECODE_H

#define EDGECODE_H

#include <fanVector.h>

enum {
    LEFT_BIT = 0,
    RIGHT_BIT = 1,
    BELOW_BIT = 2,
    ABOVE_BIT = 3,
};

#define LEFT_MASK (1<<LEFT_BIT)
#define RIGHT_MASK (1<<RIGHT_BIT)
#define BELOW_MASK (1<<BELOW_BIT)
#define ABOVE_MASK (1<<ABOVE_BIT)

// window[0] left window[1] right
// window[2] bottom window[3] top
unsigned short EncodeDot( const fan::fanVector<float, 2>& dot,
                          const fan::fanVector<float, 4>& window );

#endif /* end of include guard: EDGECODE_H */
