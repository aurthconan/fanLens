#ifndef FANLINEGENERATOR_H

#define FANLINEGENERATOR_H

#include <cstddef>

namespace fan {

template<typename T, size_t DIMENS>
class fanVector;
class fanFilm;

class fanLineGenerator
{
public:
    virtual ~fanLineGenerator();
    virtual void plotLine( const fan::fanVector<float, 2>& p1,
                           const fan::fanVector<float, 2>& p2,
                           fan::fanFilm& film );
};

}

#endif /* end of include guard: FANLINEGENERATOR_H */
