#include "fanLineGenerator_test.h"

#include <algo/rasterize/line_generator/DigitalDifferentialAnalyzer.h>

using namespace testing;

template <>
fan::fanLineGenerator* CreateLineGenerator<DigitalDifferentialAnalyzer>() {
    return new DigitalDifferentialAnalyzer();
}

INSTANTIATE_TYPED_TEST_CASE_P(DigitalDifferentialAnalyzer,
                              fanLineGenerator_test,
                              Types<DigitalDifferentialAnalyzer>);


