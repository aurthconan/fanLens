#include "fanLineGenerator.h"

namespace fan {

fanLineGenerator::~fanLineGenerator() {
}

void fanLineGenerator::plotLine( const fan::fanVector<float, 2>& p1,
                                 const fan::fanVector<float, 2>& p2,
                                 fan::fanFilm& film ) {
    (void) p1; (void) p2, (void) film;
}

}

