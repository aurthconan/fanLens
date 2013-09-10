#include "DigitalDifferentialAnalyzer.h"

#include <cmath>

#include <fanPixel.h>
#include <fanFilm.h>

using namespace std;

using namespace fan;

void DigitalDifferentialAnalyzer::plotLine( const fanVector<float, 2>& p1,
                                            const fanVector<float, 2>& p2,
                                            fanFilm& film ) {
    fanPixel pixel( 255, 255, 0, 0 );
    float deltaX = p2[0] - p1[0];
    float deltaY = p2[1] - p1[1];
    float absDeltaX = abs(deltaX);
    float absDeltaY = abs(deltaY);
    if ( absDeltaX <= 0.05 && absDeltaY <= 0.05 ) {
        film.setPixel( p1, pixel );
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
    while ( i <= length ) {
        film.setPixel( start, pixel );
        start += step;
        ++i;
    }
}

