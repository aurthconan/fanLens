#include "fanLineGenerator.h"

namespace fan {

fanLineGenerator::~fanLineGenerator() {
}

void fanLineGenerator::plotLine( const fan::fanVector<float, 2>& p1,
                                 const fan::fanVector<float, 2>& p2,
                                 const fan::fanPixel& pixel,
                                 fan::fanTexture<int, fanPixel, 2>& film,
                                 fan::fanLineGeneratorCallback* callback ) {
    (void) p1; (void) p2; (void) pixel; (void) film; (void) callback;
}

void fanLineGeneratorCallback::setStep( size_t step ) {
    (void) step;
}

}

