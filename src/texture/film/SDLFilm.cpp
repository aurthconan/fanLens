#include "SDLFilm.h"

#include <SDL_render.h>
#include <SDL_surface.h>

using namespace fan;

SDLFilm::SDLFilm( const fan::fanVector<int, 2>& dimensions )
    : fanFilm( dimensions )
    , mpSDLRenderer( NULL )
    , mpSDLWindow( NULL )
    , mpSDLTexture( NULL )
    , mpSDLSurface( NULL ) {
    if ( SDL_CreateWindowAndRenderer( dimensions[0], dimensions[1], 0,
            &mpSDLWindow, &mpSDLRenderer ) ) {
        exit(-1);
    }
    mpSDLSurface = SDL_CreateRGBSurface( 0, dimensions[0], dimensions[1],
                            32, 0x00ff0000, 0x0000ff00, 0x000000ff,
                            0xff000000 );
    mpSDLTexture = SDL_CreateTexture( mpSDLRenderer,
                            SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING,
                            dimensions[0], dimensions[1] );
}

SDLFilm::~SDLFilm() {
    SDL_FreeSurface( mpSDLSurface );
}

void SDLFilm::develope() {
    SDL_UpdateTexture( mpSDLTexture, NULL, mpSDLSurface->pixels,
                       mpSDLSurface->pitch );
    SDL_RenderClear( mpSDLRenderer );
    SDL_RenderCopy( mpSDLRenderer, mpSDLTexture, NULL, NULL );
    SDL_RenderPresent( mpSDLRenderer );
}

fanPixel SDLFilm::getPixel( fanVector<int, 2> index ) const {
    (void)index;
    return fanPixel();
}

void SDLFilm::setPixel( const fanVector<int, 2>& index,
                        const fanPixel& pixel ) {
    Uint32* bufp = (Uint32*) mpSDLSurface->pixels
                    + index[1]*mpSDLSurface->pitch/4
                    + index[0];
    *bufp = SDL_MapRGBA( mpSDLSurface->format,
                pixel.r, pixel.g, pixel.b, pixel.a );
}

