#ifndef FILM_TEST_H

#define FILM_TEST_H

#include <gtest/gtest.h>

#include <fanFilm.h>
#include <fanVector.h>
#include <fanPixel.h>

template <class T>
fan::fanFilm* CreateFilm( const fan::fanVector<int, 2>& dimensions );

template <class T>
void DeleteFilm( fan::fanFilm* film ) {
    delete film;
}

template<typename T>
class Film_test
    : public testing::Test
{
public:
    Film_test() : createFilm( CreateFilm<T> ) , deleteFilm( DeleteFilm<T> ) {};
    virtual ~Film_test() {};

    fan::fanFilm* (*createFilm)( const fan::fanVector<int, 2>& dimensions );
    void (*deleteFilm)( fan::fanFilm* film );
};

TYPED_TEST_CASE_P(Film_test);

#if GTEST_HAS_TYPED_TEST

TYPED_TEST_P(Film_test, AllColorTestCard) {
    fan::fanVector<int, 2> size;
    size[0] = 4096; size[1] = 4096;
    fan::fanFilm* film = this->createFilm( size );
    if ( !film ) {
        FAIL() << "film not instantiate";
        return;
    }
    fan::fanVector<int, 2> pos;
    for ( size_t i = 0; i < 4096; ++i ) {
        for ( size_t j = 0; j < 4096; ++j ) {
            unsigned char r = i % 256;
            unsigned char g = j % 256;
            unsigned char b = j / 256 * 16 + i / 256;
            pos[0] = i; pos[1] = j;
            film->setPixel( pos, fan::fanPixel( 255, r, g, b ) );
        }
    }
    this->deleteFilm( film );
}

REGISTER_TYPED_TEST_CASE_P(
    Film_test,
    AllColorTestCard);

#endif

#endif /* end of include guard: FILM_TEST_H */
