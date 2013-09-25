#ifndef FREEIMAGEFILM_H

#define FREEIMAGEFILM_H

#include <fanFilm.h>
#include <string>
#include <FreeImage.h>

/* Save image to file use freeimage library */

struct FIBITMAP;

class FreeImageFilm
    : public fan::fanFilm
{
public:
    FreeImageFilm( const fan::fanVector<int, 2>& dimensions,
                   const std::string file );
    ~FreeImageFilm();

    fan::fanPixel getValue( fan::fanVector<int, 2> index ) const;
    void setValue( const fan::fanVector<int, 2>& index,
                   const fan::fanPixel& pixel );

    void develope();

private:
    static bool sbFreeImageInitialized;
    std::string mFileName;
    FIBITMAP* mpBitmap;
};

#endif /* end of include guard: FREEIMAGEFILM_H */
