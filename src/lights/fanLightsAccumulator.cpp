#include "fanLightsAccumulator.h"

using namespace std;
using namespace boost;

namespace fan {

fanLightsAccumulator::fanLightsAccumulator( vector<boost::shared_ptr<fanLight> >& lights )
    : mLights( lights )
{
}

fanPixel fanLightsAccumulator::getLight( fan::fanVector3<float> world,
                                         fan::fanVector3<float> normal,
                                         fan::fanVector3<float> viewer ) {
    fanPixel result( 255, 0, 0, 0 );
    fanPixel white( 255, 255, 255, 255 );
    for ( auto itor = mLights.begin(), end = mLights.end();
            itor != end; ++itor ) {
        fanPixel value = (*itor)->getLight( world, normal, viewer );
        result += value;
        if ( result == white ) {
            break;
        }
    }
    return result;
}

} /* namespace fan */

