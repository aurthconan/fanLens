#include <gtest/gtest.h>

#include <texture/film/FreeImageFilm.h>

TEST(FreeImageFilm,GeneratePic) {
    fan::fanVector<int, 2> size;
    size[0] = 4; size[1] = 4;
    FreeImageFilm film( size, "test.png" );

    size[1] = 0;
    for ( size_t i = 0; i < 4; ++i ) {
        size[0] = i;
        film.setPixel( size, fan::fanPixel(255, 255, 0, 0) );
    }
}

