#ifndef SDLFONT
#define SDLFONT

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL2/SDL.h>

#include <string>

#include "ErrorInfo.h"

#define TEXTUPPER 0
#define TEXTLOWER 1
#define TEXTMIXED 2
#define TEXTALIGNLEFT 10
#define TEXTALIGNRIGHT 11
#define TEXTALIGNCENTER 12

#define SDLFONTSIZE 16

class CustomFont {
public:
    void ClearFont();
    LPErrInApp LoadFont(const char *filename);
    void DrawString(SDL_Surface *surface, std::string message,
                    unsigned char textcase, char alignment, int x, int y,
                    unsigned int color);
    CustomFont();
    ~CustomFont();

private:
    SDL_Surface *_p_Bitmap;
    std::string _Filename;
    SDL_Rect _rSource;
    SDL_Rect _rTarget;
};

#endif
