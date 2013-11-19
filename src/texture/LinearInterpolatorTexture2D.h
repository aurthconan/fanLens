#ifndef LINEARINTERPOLATORTEXTURE2D_H

#define LINEARINTERPOLATORTEXTURE2D_H

#include <fanTexture.h>

template<typename T_VALUE>
class LinearInterpolatorTexture2D
    : public fan::fanTexture<float, T_VALUE, 2>
{
public:
    LinearInterpolatorTexture2D( fan::fanTexture<int, T_VALUE, 2>& texture )
        : fan::fanTexture<float, T_VALUE, 2> (
               &texture==NULL?fan::fanVector<int,2>()
                            :texture.getDimens() )
        , mpTexture( &texture )
        {}

    virtual T_VALUE getValue( fan::fanVector<float, 2> index ) const {
        fan::fanVector<int, 2> _00 = index;
        fan::fanVector<int, 2> _01 = _00 + fan::fanVector<int,2>{0,1};
        fan::fanVector<int, 2> _10 = _00 + fan::fanVector<int,2>{1,0};
        fan::fanVector<int, 2> _11 = _00 + fan::fanVector<int,2>{1,1};

        float _0Ratio = index[0] - (float)_00[0];
        float _1Ratio = index[1] - (float)_00[1];
        T_VALUE _0 = (1.0 - _0Ratio) * mpTexture->getValue( _00 )
                          + _0Ratio  * mpTexture->getValue( _01 );
        T_VALUE _1 = (1.0 - _0Ratio) * mpTexture->getValue( _10 )
                          + _0Ratio  * mpTexture->getValue( _11 );
        return (1.0 - _1Ratio) * _0 + _1Ratio * _1;
    }

    virtual void setValue( const fan::fanVector<float, 2>& index,
                           const T_VALUE& value ) {
        (void) index; (void) value;
        return;
    }

private:
    fan::fanTexture<int, T_VALUE, 2>* mpTexture;
};

#endif /* end of include guard: LINEARINTERPOLATORTEXTURE2D_H */
