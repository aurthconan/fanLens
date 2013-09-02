#include "Camera_test.h"

#include <camera/PointScannerCamera.h>

using namespace testing;

template <>
fan::fanCamera* CreateCamera<PointScannerCamera>() {
    return new PointScannerCamera();
}

INSTANTIATE_TYPED_TEST_CASE_P(PointScanner,
                              Camera_test,
                              Types<PointScannerCamera>);

