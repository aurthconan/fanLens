#ifndef COHENSUTHERLAND_H

#define COHENSUTHERLAND_H

#include <fanVector.h>

bool CohenSutherland( const fan::fanVector<float, 4>& window,
                      fan::fanVector<float, 2>& p1,
                      fan::fanVector<float, 2>& p2 );

#endif /* end of include guard: COHENSUTHERLAND_H */
