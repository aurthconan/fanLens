#include "Film_test.h"

#include <texture/film/FreeImageFilm.h>
#include <texture/film/SDLFilm.h>

using namespace testing;

template <>
fan::fanFilm* CreateFilm<FreeImageFilm>( const fan::fanVector<int, 2>& dimensions ) {
    return new FreeImageFilm( dimensions, "test.png" );
}

INSTANTIATE_TYPED_TEST_CASE_P(FreeImage,
                              Film_test,
                              Types<FreeImageFilm>);

template <>
fan::fanFilm* CreateFilm<SDLFilm>( const fan::fanVector<int, 2>& dimensions ) {
    return new SDLFilm( dimensions );
}

INSTANTIATE_TYPED_TEST_CASE_P(SDL,
                              Film_test,
                              Types<SDLFilm>);

