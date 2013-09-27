#ifndef FANLINEGENERATOR_H

#define FANLINEGENERATOR_H

#include <cstddef>

namespace fan {

template<typename T, size_t DIMENS>
class fanVector;
class fanFilm;
class fanPixel;
template<typename T, typename T_VALUE, size_t DIMENS>
class fanTexture;

class fanLineGeneratorCallback
{
public:
    virtual void setStep( size_t step );
};

class fanLineGenerator
{
public:
    virtual ~fanLineGenerator();
    virtual void plotLine( const fan::fanVector<float, 2>& p1,
                           const fan::fanVector<float, 2>& p2,
                           const fan::fanPixel& pixel,
                           fan::fanTexture<int, fan::fanPixel, 2>& texture,
                           fan::fanLineGeneratorCallback* callback );
};

}

#endif /* end of include guard: FANLINEGENERATOR_H */
