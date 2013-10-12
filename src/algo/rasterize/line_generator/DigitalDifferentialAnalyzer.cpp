#include "DigitalDifferentialAnalyzer.h"

#include <cmath>

#include <fanPixel.h>
#include <fanTexture.h>

using namespace std;

using namespace fan;

void DigitalDifferentialAnalyzer::plotLine( const fanVector<float, 2>& p1,
                                            const fanVector<float, 2>& p2,
                                            const fanPixel& pixel,
                                            fanTexture<int, fanPixel, 2>& film,
                                            fanLineGeneratorCallback* callback ) {
    float deltaX = p2[0] - p1[0];
    float deltaY = p2[1] - p1[1];
    float absDeltaX = abs(deltaX);
    float absDeltaY = abs(deltaY);
    if ( absDeltaX <= 0.05 && absDeltaY <= 0.05 ) {
        film.setValue( p1, pixel );
        return;
    }
    int length = 0;
    if ( absDeltaX >= absDeltaY ) {
        length = (int) absDeltaX;
    } else {
        length = (int) absDeltaY;
    }

    fanVector<float, 2> step;
    step[0] = deltaX / length;
    step[1] = deltaY / length;

    fanVector<float, 2> start = p1;
    int i = 0;
    if ( callback ) {
        callback->setStep( (size_t) length );
    }
    while ( i <= length ) {
        film.setValue( start, pixel );
        start += step;
        ++i;
    }
}

