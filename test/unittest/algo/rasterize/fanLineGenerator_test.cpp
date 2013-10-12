#include "fanLineGenerator_test.h"

#include <algo/rasterize/line_generator/DigitalDifferentialAnalyzer.h>
#include <algo/rasterize/line_generator/Bresenham.h>

using namespace testing;

CREATE_LINE_GENERATOR(DigitalDifferentialAnalyzer);
GET_NAME(DigitalDifferentialAnalyzer);

CREATE_LINE_GENERATOR(Bresenham);
GET_NAME(Bresenham);

typedef Types<DigitalDifferentialAnalyzer, Bresenham>
        LineGeneratorImplementation;

INSTANTIATE_TYPED_TEST_CASE_P(DigitalDifferentialAnalyzer,
                              fanLineGenerator_test,
                              LineGeneratorImplementation);

