#ifndef __GFX_UTIL__H
#define __GFX_UTIL__H

#include <SDL2/SDL.h>

#include <string>

#ifdef WIN32
#define DATA_PREFIX "./data/"
#define VERSION "0.1"
#endif

namespace GFX_UTIL {
    Uint32 inline GetPixel(SDL_Surface *surface, int x, int y);
    void inline SetPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);

    void rectangleRGBA(SDL_Surface *screen, int x, int y, int x2, int y2,
                       unsigned char r, unsigned char g, unsigned char b,
                       unsigned char a);
    void boxRGBA(SDL_Surface *screen, int x1, int y1, int x2, int y2,
                 unsigned char r, unsigned char g, unsigned char b,
                 unsigned char a);

    void ScreenShade(SDL_Surface *surface, SDL_Rect *rect, float opacity,
                     unsigned char r, unsigned char g, unsigned char b);

};

namespace STR_UTIL {
    std::string intToString(int iNumber);
};

#endif
