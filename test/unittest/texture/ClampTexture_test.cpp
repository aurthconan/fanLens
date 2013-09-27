#include <gtest/gtest.h>

#include <texture/ClampTexture.h>

using namespace fan;

class ClampTextureTestStub1D
    : public fanTexture<int, int, 1>
{
public:
    ClampTextureTestStub1D( const fanVector<int, 1>& dimensions )
        : fanTexture<int, int, 1>( dimensions ) {
    }

    virtual int getValue( fanVector<int, 1> index ) const {
        fanVector<int, 1> dimens = getDimens();
        if ( index[0] < 0 || index[0] >= dimens[0] ) {
            ADD_FAILURE() << " index[0] " << index[0]
                          << " index[1] " << index[1];
        }
        return 0;
    }

    virtual void setValue( const fanVector<int, 1>& index,
                           const int& value ) {
        fanVector<int, 1> dimens = getDimens();
        if ( index[0] < 0 || index[0] >= dimens[0] ) {
            ADD_FAILURE() << " index[0] " << index[0]
                          << " index[1] " << index[1];
        }
        return;
    }

};

TEST(ClampTexture, 1d) {
    fanVector<int, 1> size;
    size[0] = 5;
    ClampTextureTestStub1D stub( size );

    ClampTexture<int, int, 1> clamp( &stub );

    fanVector<int, 1> pos;
    for ( int i = -5; i < 10; ++i ) {
        pos[0] = i;
        clamp.setValue( pos, 1 );
    }

    for ( int i = -5; i < 10; ++i ) {
        pos[0] = i;
        clamp.getValue( pos );
    }
}
