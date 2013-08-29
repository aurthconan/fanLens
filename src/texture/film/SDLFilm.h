#ifndef SDLFILM_H

#define SDLFILM_H

#include <fanFilm.h>

class SDLFilm
    : public fan::fanFilm
{
public:
    SDLFilm( const fan::fanVector<int, 2>& dimensions );
    ~SDLFilm();

    fan::fanPixel getPixel( fan::fanVector<int, 2> index ) const;
    void setPixel( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel );

};

#endif /* end of include guard: SDLFILM_H */
