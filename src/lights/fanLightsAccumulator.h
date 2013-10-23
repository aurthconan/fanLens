#ifndef FANLIGHTSACCUMULATOR_H

#define FANLIGHTSACCUMULATOR_H

#include <fanLight.h>
#include <fanPixel.h>

#include <vector>
#include <boost/shared_ptr.hpp>

namespace fan {

class fanLightsAccumulator
    : public fanLight
{
public:
    fanLightsAccumulator( std::vector<boost::shared_ptr<fanLight> >& lights );
    virtual fan::fanPixel getLight( fan::fanVector3<float> world,
                                    fan::fanVector3<float> normal,
                                    fan::fanVector3<float> viewer );
private:
    std::vector<boost::shared_ptr<fanLight> >& mLights;
};

} /* namespace fan */

#endif /* end of include guard: FANLIGHTSACCUMULATOR_H */
