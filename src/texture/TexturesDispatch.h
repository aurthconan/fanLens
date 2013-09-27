#ifndef TEXTURESDISPATCH_H

#define TEXTURESDISPATCH_H

#include <fanTexture.h>
#include <vector>

template<typename T, typename T_VALUE, size_t DIMENS>
class TexturesDispatch
    : public fan::fanTexture<T, T_VALUE, DIMENS>
{
public:
    TexturesDispatch( const fanVector<T, DIMENS>& dimensions )
        : fan::fanTexture<T, T_VALUE, DIMENS> ( dimensions ) {
    }
    ~TexturesDispatch() {
    }

    virtual void setValue( const fanVector<T, DIMENS>& index,
                           const T_VALUE& value ) {
        for ( auto itor = mTextures.begin(), max = mTextures.end();
                itor != max; ++itor ) {
            (*itor)->setValue( index, value );
        }
    }

    void addTextures( fan::fanTexture<T, T_VALUE, DIMENS>* tex ) {
        if ( tex == NULL ) {
            return;
        }
        mTextures.push_back( mTextures );
    }

private:
    std::vector<fan::fanTexture<T, T_VALUE, DIMENS>*> mTextures;
};


#endif /* end of include guard: TEXTURESDISPATCH_H */
