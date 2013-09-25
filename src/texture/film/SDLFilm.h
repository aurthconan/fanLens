#ifndef SDLFILM_H

#define SDLFILM_H

#include <fanFilm.h>

extern "C" {
struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;
struct SDL_Surface;
}

class SDLFilm
    : public fan::fanFilm
{
public:
    SDLFilm( const fan::fanVector<int, 2>& dimensions );
    ~SDLFilm();

    fan::fanPixel getValue( fan::fanVector<int, 2> index ) const;
    void setValue( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel );

    void develope();
private:
    SDL_Renderer* mpSDLRenderer;
    SDL_Window* mpSDLWindow;
    SDL_Texture* mpSDLTexture;
    SDL_Surface* mpSDLSurface;
};

#endif /* end of include guard: SDLFILM_H */
