#ifndef SDLFONT
#define SDLFONT

#ifdef _WINDOWS
#pragma warning(disable : 4786)
#endif

#include <SDL.h>

#include <string>

using namespace std;

#define TEXTUPPER 0
#define TEXTLOWER 1
#define TEXTMIXED 2
#define TEXTALIGNLEFT 10
#define TEXTALIGNRIGHT 11
#define TEXTALIGNCENTER 12

#define SDLFONTSIZE 16

class CustomFont {
private:
    SDL_Surface *Bitmap;
    string Filename;
    SDL_Rect rSource;
    SDL_Rect rTarget;

public:
    void ClearFont();
    bool LoadFont(string filename);
    void DrawString(SDL_Surface *surface, string message,
                    unsigned char textcase, char alignment, int x, int y,
                    unsigned int color);
    CustomFont();
    ~CustomFont();
};

#endif
