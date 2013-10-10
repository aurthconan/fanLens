#ifndef CLAMPTEXTURE_H

#define CLAMPTEXTURE_H

#include <fanTexture.h>

template<typename T, typename T_VALUE, size_t DIMENS>
class ClampTexture
    : public fan::fanTexture<T, T_VALUE, DIMENS>
{
public:
    ClampTexture( fan::fanTexture<T, T_VALUE, DIMENS>* texture )
        : fan::fanTexture<T, T_VALUE, DIMENS> (
               texture==NULL?fan::fanVector<T,DIMENS>()
                            :texture->getDimens() )
        , mpTexture( texture ) {
    }

    virtual T_VALUE getValue( fan::fanVector<T, DIMENS> index ) const {
        if ( !mpTexture || !inRange( index ) ) {
            return T_VALUE();
        }
        return mpTexture->getValue( index );
    }

    virtual void setValue( const fan::fanVector<T, DIMENS>& index,
                           const T_VALUE& value ) {
        if ( !mpTexture || !inRange( index ) ) {
            return;
        }
        mpTexture->setValue( index, value );
    }

private:
    bool inRange( const fan::fanVector<T, DIMENS>& index ) const {
        const fan::fanVector<T, DIMENS>& dimens =
            fan::fanTexture<T, T_VALUE, DIMENS>::getDimens();
        for ( size_t i = 0; i < DIMENS; ++i ) {
            if ( index[i] < 0 || index[i] >= dimens[i] ) {
                return false;
            }
        }
        return true;
    }

    fan::fanTexture<T, T_VALUE, DIMENS>* mpTexture;
};

#endif /* end of include guard: CLAMPTEXTURE_H */
