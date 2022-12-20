#include "gfx_util.h"

void GFX_UTIL::ScreenShade(SDL_Surface *surface, SDL_Rect *rect, float opacity,
                           unsigned char r1, unsigned char g1,
                           unsigned char b1) {
    Uint32 pixel;
    Uint8 r, g, b;
    bool bunlock = false;
    if (surface == NULL)
        return;
    if (SDL_LockSurface(surface) == 0)
        bunlock = true;
    // SET TRANSPARENCY FOR THE BOARD AREA
    for (int y = rect->y; y < rect->y + rect->h; y++)
        for (int x = rect->x; x < rect->x + rect->w; x++) {
            pixel = GetPixel(surface, x, y);
            SDL_GetRGB(pixel, surface->format, &r, &g, &b);
            r = (Uint8)((float)r * opacity);
            g = (Uint8)((float)g * opacity);
            b = (Uint8)((float)b * opacity);
            SetPixel(surface, x, y, SDL_MapRGBA(surface->format, r, g, b, 255));
        }
    if (bunlock == true)
        SDL_UnlockSurface(surface);
    rectangleRGBA(surface, rect->x, rect->y, rect->x + rect->w,
                  rect->y + rect->h, 0, 0, 0, 255);
    rectangleRGBA(surface, rect->x + 1, rect->y + 1, rect->x + rect->w - 2,
                  rect->y + rect->h - 2, r1, g1, b1, 255);
}

void GFX_UTIL::rectangleRGBA(SDL_Surface *screen, int x1, int y1, int x2,
                             int y2, unsigned char r, unsigned char g,
                             unsigned char b, unsigned char a) {
    int x, y;
    Uint32 color;
    bool bneedunlock = false;
    if (screen == NULL)
        return;
    if (SDL_LockSurface(screen) == 0)
        bneedunlock = true;

    color = SDL_MapRGBA(screen->format, r, g, b, a);

    for (x = x1; x < x2; x++) {
        SetPixel(screen, x, y1, color);
        SetPixel(screen, x, y2 - 1, color);
    }

    for (y = y1; y < y2; y++) {
        SetPixel(screen, x1, y, color);
        SetPixel(screen, x2 - 1, y, color);
    }
    if (bneedunlock == true)
        SDL_UnlockSurface(screen);
}

void GFX_UTIL::boxRGBA(SDL_Surface *screen, int x1, int y1, int x2, int y2,
                       unsigned char r, unsigned char g, unsigned char b,
                       unsigned char a) {
    int x, y;
    Uint32 color;
    bool bneedunlock = false;
    if (screen == NULL)
        return;

    if (SDL_LockSurface(screen) == 0)
        bneedunlock = true;

    color = SDL_MapRGBA(screen->format, r, g, b, a);

    for (y = y1; y < y2; y++) {
        for (x = x1; x < x2; x++) {
            SetPixel(screen, x, y, color);
        }
    }
    if (bneedunlock == true)
        SDL_UnlockSurface(screen);
}

Uint32 inline GFX_UTIL::GetPixel(SDL_Surface *surface, int x, int y) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;

        default:
            return 0; /* shouldn't happen, but avoids warnings */
    }
}
void inline GFX_UTIL::SetPixel(SDL_Surface *surface, int x, int y,
                               Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

std::string STR_UTIL::intToString(int iNumber) {
    std::string temp;
    char buffer[15];
    sprintf(buffer, "%d", iNumber);
    temp = buffer;
    return temp;
}
