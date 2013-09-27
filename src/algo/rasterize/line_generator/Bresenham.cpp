#include "Bresenham.h"

#include <cmath>

#include <fanPixel.h>
#include <fanTexture.h>

using namespace std;

using namespace fan;

namespace {
template <typename T> inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
}

void Bresenham::plotLine( const fanVector<float, 2>& p1,
                          const fanVector<float, 2>& p2,
                          const fanPixel& pixel,
                          fanTexture<int, fanPixel, 2>& film,
                          fan::fanLineGeneratorCallback* callback ) {
    fanVector<float, 2> point = p1;
    float deltaX = p2[0] - p1[0];
    float deltaY = p2[1] - p1[1];
    float absX = abs(deltaX);
    float absY = abs(deltaY);
    int signX = sgn(deltaX);
    int signY = sgn(deltaY);

    bool interchange = false;
    if ( absY > absX ) {
        float temp = absX;
        absX = absY;
        absY = temp;
        interchange = true;
    }
    int err = 2 * absY - absX;
    if ( callback ) {
        callback->setStep( (size_t) absX );
    }
    for ( size_t i = 0, max = (size_t)absX; i <= max; ++i ) {
        film.setValue( point, pixel );
        while ( err > 0 ) {
            if ( interchange ) {
                point[0] += signX;
            } else {
                point[1] += signY;
            }
            err -= 2*absX;
        }
        if ( interchange ) {
            point[1] += signY;
        } else {
            point[0] += signX;
        }
        err += 2*absY;
    }
}

