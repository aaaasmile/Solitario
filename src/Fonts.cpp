#include "Fonts.h"

#include "win_type_global.h"

CustomFont::CustomFont() { _p_Bitmap = NULL; }
CustomFont::~CustomFont() { ClearFont(); }
void CustomFont::ClearFont() {
    if (_p_Bitmap != NULL) {
        SDL_FreeSurface(_p_Bitmap);
        _p_Bitmap = NULL;
    }
}

LPErrInApp CustomFont::LoadFont(const char* filename) {
    ClearFont();
    _p_Bitmap = SDL_LoadBMP(filename);
    if (_p_Bitmap == 0) {
        return ERR_UTIL::ErrorCreate("Font %s not  found\n", filename);
    }
    SDL_SetColorKey(_p_Bitmap, TRUE, SDL_MapRGB(_p_Bitmap->format, 0, 0, 0));
    _Filename = filename;
    return NULL;
}

void CustomFont::DrawString(SDL_Surface* surface, std::string message,
                            unsigned char textcase, char alignment, int x,
                            int y, unsigned int color) {
    SDL_Rect rctTarget;
    SDL_Rect rctSource;
    unsigned char c;
    int ax;
    rctSource.w = SDLFONTSIZE;
    rctSource.h = SDLFONTSIZE;
    rctTarget.w = SDLFONTSIZE;
    rctTarget.h = SDLFONTSIZE;

    switch (alignment) {
        case TEXTALIGNLEFT:
            ax = 0;
            break;
        case TEXTALIGNRIGHT:
            ax = surface->w - ((int)message.size() * SDLFONTSIZE);
            break;
        case TEXTALIGNCENTER:
            ax = (surface->w / 2) - (((int)message.size() * SDLFONTSIZE) / 2);
            break;
        default:
            ax = 0;
            break;
    };

    for (unsigned int k = 0; k < message.size(); k++) {
        rctTarget.y = y;
        rctTarget.x = x + k * SDLFONTSIZE + ax;
        c = message[k];
        switch (textcase) {
            case TEXTUPPER:
                if ((c >= 'a') && (c <= 'z'))
                    c -= 32;
                break;
            case TEXTLOWER:
                if ((c >= 'A') && (c <= 'Z'))
                    c += 32;
                break;
            default:
                break;
        };
        if (c < 32)
            c = 0;
        if (c != 0) {
            rctSource.x = SDLFONTSIZE * (c % 16);
            rctSource.y = SDLFONTSIZE * (c / 16);
            SDL_BlitSurface(_p_Bitmap, &rctSource, surface, &rctTarget);
        }
    }
}
