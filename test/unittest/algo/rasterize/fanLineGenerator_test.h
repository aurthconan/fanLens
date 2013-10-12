#ifndef FANLINEGENERATOR_TEST_H

#define FANLINEGENERATOR_TEST_H

#include <gtest/gtest.h>

#include <algo/rasterize/fanLineGenerator.h>

#include <texture/film/FreeImageFilm.h>

#include <string.h>

template <class T>
fan::fanLineGenerator* CreateLineGenerator() {
    return NULL;
};

template <class T>
const char* GetName() {
    return NULL;
}

#define GET_NAME(T)             \
    template <>                 \
    const char* GetName<T>() {  \
        return #T;              \
    }
#define CREATE_LINE_GENERATOR(T)                        \
    template <>                                         \
    fan::fanLineGenerator* CreateLineGenerator<T>() {   \
        return new T();                                 \
    }

template <class T>
void DeleteLineGenerator( fan::fanLineGenerator* lineGenerator ) {
    delete lineGenerator;
}

template<typename T>
class fanLineGenerator_test
    : public testing::Test
{
public:
    fanLineGenerator_test()
        : createLineGenerator( CreateLineGenerator<T> )
        , deleteLineGenerator( DeleteLineGenerator<T> )
        , getName( GetName<T> ) {};
    virtual ~fanLineGenerator_test() {};

    fan::fanLineGenerator* (*createLineGenerator)();
    void (*deleteLineGenerator)( fan::fanLineGenerator* lineGenerator );
    const char* (*getName)();
};

TYPED_TEST_CASE_P(fanLineGenerator_test);

#if GTEST_HAS_TYPED_TEST

TYPED_TEST_P(fanLineGenerator_test,DiagonalFromBottomLeftToTopRight) {
    fan::fanVector<int, 2> size;
    fan::fanPixel pixel( 255, 255, 0, 0 );
    size[0] = 100; size[1] = 100;
    std::string fileName = std::string(this->getName())
                            + "DiagonalFromBottomLeftToTopRight.png";
    FreeImageFilm film( size, fileName );

    fan::fanLineGenerator* lineGenerator = this->createLineGenerator();
    if ( !lineGenerator ) {
        FAIL() << "lineGenerator not instantiate";
        return;
    }

    fan::fanVector<float, 2> p1, p2;
    p2[1] = 99; p2[0] = 99;
    lineGenerator->plotLine( p1, p2, pixel, film, NULL );
    film.develope();

    this->deleteLineGenerator( lineGenerator );
}

REGISTER_TYPED_TEST_CASE_P(
    fanLineGenerator_test,
    DiagonalFromBottomLeftToTopRight);

#endif

#endif /* end of include guard: FANLINEGENERATOR_TEST_H */
