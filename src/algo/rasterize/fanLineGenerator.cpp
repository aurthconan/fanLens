#include "fanLineGenerator.h"

namespace fan {

fanLineGenerator::~fanLineGenerator() {
}

void fanLineGenerator::plotLine( const fan::fanVector<float, 2>& p1,
                                 const fan::fanVector<float, 2>& p2,
                                 const fan::fanPixel& pixel,
                                 fan::fanFilm& film ) {
    (void) p1; (void) p2; (void) pixel; (void) film;
}

}

