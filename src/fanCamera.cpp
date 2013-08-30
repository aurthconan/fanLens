#include "fanCamera.h"

namespace fan {

fanCamera::~fanCamera() {
}

void fanCamera::takePicture( fanScene& scene,
                             fanFilm& film,
                             fanLens& lens ) {
    (void)scene; (void)film; (void)lens;
}

}
