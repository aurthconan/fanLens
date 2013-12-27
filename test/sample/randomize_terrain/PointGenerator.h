#ifndef POINTGENERATOR_H

#define POINTGENERATOR_H

#include <fanVector3.h>
#include <vector>

class PointGenerator
{
public:
    class PointIndex {
        public:
            PointIndex( size_t level, size_t x, size_t y );
            size_t mConvertedLevel;
            size_t mConvertedX;
            size_t mConvertedY;

            size_t mOriginalLevel;
            size_t mOriginalX;
            size_t mOriginalY;
        private:
            PointIndex();
    };
    PointGenerator( size_t level = 0, PointGenerator* parent = NULL,
                    fan::fanVector3<float> a = fan::fanVector3<float>(),
                    fan::fanVector3<float> b = fan::fanVector3<float>(),
                    fan::fanVector3<float> c = fan::fanVector3<float>(),
                    fan::fanVector3<float> d = fan::fanVector3<float>() );
    ~PointGenerator();

    fan::fanVector3<float> getPoint( const PointIndex& index );
private:
    PointGenerator();
    const size_t mLevel;
    static size_t computeIndex( const PointIndex& index );

    PointGenerator* mHigherLevel;
    PointGenerator* mLowerLevel;
    std::vector<fan::fanVector3<float> > mPoints;
    std::vector<bool> mGenerated;
};

#endif /* end of include guard: POINTGENERATOR_H */
