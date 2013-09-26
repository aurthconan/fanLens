#include <gtest/gtest.h>

#include <texture/MemoryTexture.h>

using namespace fan;

TEST(MemoryTexture,1d) {
    fanVector<int, 1> size;
    size[0] = 5;
    MemoryTexture<int, int, 1> texture(size);
    fanVector<int, 1> pos;
    for ( size_t i = 0, max = (size_t)size[0];
                i < max; ++i ) {
        pos[0] = i;
        texture.setValue( pos, i );
    }
    for ( size_t i = 0, max = (size_t)size[0];
                i < max; ++i ) {
        pos[0] = i;
        EXPECT_EQ( i, texture.getValue(pos) );
    }
}

TEST(MemoryTexture,2d) {
    fanVector<int, 2> size;
    size[0] = 5; size[1] = 5;
    MemoryTexture<int, int, 2> texture(size);
    fanVector<int, 2> pos;
    for ( size_t i = 0, iMax = (size_t)size[0];
                i < iMax; ++i ) {
        pos[0] = i;
        for ( size_t j = 0, jMax = (size_t) size[1];
                j < jMax; ++j ) {
            pos[1] = j;
            texture.setValue( pos, i*j+i );
        }
    }
    for ( size_t i = 0, iMax = (size_t)size[0];
                i < iMax; ++i ) {
        pos[0] = i;
        for ( size_t j = 0, jMax = (size_t) size[1];
                j < jMax; ++j ) {
            pos[1] = j;
            EXPECT_EQ( i*j+i, texture.getValue(pos) );
        }
    }
}

