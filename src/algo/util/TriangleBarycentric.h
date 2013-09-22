#ifndef TRIANGLEBARYCENTRIC_H

#define TRIANGLEBARYCENTRIC_H

#include <fanVector.h>

namespace fan {

template<typename T>
inline fanVector<T, 3> ComputeTriangleBarycentricCoord( fanVector<T, 2> p1,
                                                        fanVector<T, 2> p2,
                                                        fanVector<T, 2> p3,
                                                        fanVector<T, 2> p ) {
    T det = (p2[1]-p3[1])*(p1[0]-p3[0])+(p3[0]-p2[0])*(p1[1]-p3[1]);
    fanVector<T, 3> result;
    if ( det == T(0) ) {
        return result;
    }
    result[0] = ((p2[1]-p3[1])*(p[0]-p3[0])+(p3[0]-p2[0])*(p[1]-p3[1]))/det;
    result[1] = ((p3[1]-p1[1])*(p[0]-p3[0])+(p1[0]-p3[0])*(p[1]-p3[1]))/det;
    result[2] = 1 - result[0] - result[1];
    return result;
}

}

#endif /* end of include guard: TRIANGLEBARYCENTRIC_H */
